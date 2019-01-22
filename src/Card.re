module Styles = {
  open Css;
  let card =
    style([
      border(px(2), solid, hex("000000")),
      borderRadius(px(4)),
      padding(rem(1.0)),
      marginBottom(px(8)),
    ]);
};

let maxDescriptionLength = 300;
let getMin = (a: int, b: int): int => a < b ? a : b;

let component = ReasonReact.statelessComponent("Card");

let make = (~name, ~description, ~href, _children) => {
  ...component,
  render: _self =>
    <div className=Styles.card>
      <h3>
        <a href target="_blank" rel="noopener noreferrer">
          {ReasonReact.string(name)}
        </a>
      </h3>
      <p>
        {String.length(description) < maxDescriptionLength ?
           ReasonReact.string(description) :
           ReasonReact.string(
             String.sub(
               description,
               0,
               getMin(String.length(description), maxDescriptionLength),
             )
             ++ "...",
           )}
      </p>
    </div>,
};