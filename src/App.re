/**
 * MODULE: Api
 * Define the data type, decoder, and the data fetcher
 */
type repository = {
  name: string,
  description: string,
  href: string,
};

module Api = {
  open Json.Decode;
  let decodeResults =
    field(
      "items",
      list(
        optional(json =>
          {
            name: field("name", string, json),
            description: field("description", string, json),
            href: field("html_url", string, json),
          }
        ),
      ),
    );
  let getResults = query =>
    Js.Promise.(
      Fetch.fetch("https://api.github.com/search/repositories?q=" ++ query)
      |> then_(Fetch.Response.json)
      |> then_(json => decodeResults(json) |> resolve)
      |> then_(results =>
           results
           |> List.filter(optionalItem =>
                switch (optionalItem) {
                | Some(_) => true
                | None => false
                }
              )
           |> List.map(item =>
                switch (item) {
                | Some(item) => item
                | _ => raise(Not_found)
                }
              )
           |> resolve
         )
    );
};

/**
 * React Component Definition: App
 */
type state = {
  input: string,
  isLoading: bool,
  results: list(repository),
};

type action =
  | UpdateInput(string)
  | Search
  | UpdateResults(list(repository));

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  let onFormSubmit = (ev, self) => {
    ReactEvent.Form.preventDefault(ev);
    /* Use ReasonReact.() to give context of self */
    ReasonReact.(self.send(Search));
  };

  let onInputChange = (ev, self) => {
    let value = ReactEvent.Form.target(ev)##value;
    /* Use ReasonReact.() to give context of self */
    ReasonReact.(self.send(UpdateInput(value)));
  };
  {
    ...component,
    initialState: () => {input: "", isLoading: false, results: []},
    reducer: (action, state) =>
      switch (action) {
      | UpdateInput(newInput) =>
        ReasonReact.Update({...state, input: newInput})
      | UpdateResults(results) =>
        ReasonReact.Update({...state, isLoading: false, results})
      | Search =>
        ReasonReact.UpdateWithSideEffects(
          {...state, isLoading: true},
          self => {
            let value = self.state.input;
            let _ =
              Api.getResults(value)
              |> Js.Promise.then_(results => {
                   self.send(UpdateResults(results));
                   Js.Promise.resolve();
                 });
            /* Return unit because this function does not return the promise */
            ();
          },
        )
      },
    render: self => {
      <div>
        <form onSubmit={self.handle(onFormSubmit)}>
          <label htmlFor="search">
            {ReasonReact.string("Search GitHub projects: ")}
          </label>
          <input
            id="search"
            name="search"
            value={self.state.input}
            onChange={self.handle(onInputChange)}
          />
          <button type_="submit"> {ReasonReact.string("Search")} </button>
        </form>
        <div>
          {self.state.isLoading ?
             ReasonReact.string("Loading...") :
             self.state.results
             |> Array.of_list
             |> Array.map(({name, href, description}) =>
                  <Card key=name name href description />
                )
             |> ReasonReact.array}
        </div>
      </div>;
    },
  };
};