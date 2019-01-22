module Index = {
  Css.(
    global(
      "body",
      [
        fontFamily(
          "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif",
        ),
      ],
    )
  );
  Css.(global("a", [color(hex("1976D2")), textDecoration(none)]));
  Css.(global("a:visited", [color(hex("512DA8")), textDecoration(none)]));
  Css.(global("h1, h2, h3, p", [margin(px(0)), marginBottom(px(8))]));
};

ReactDOMRe.renderToElementWithId(
  <Card name="Reason" description="This is cool" href="" />,
  "Card",
);

ReactDOMRe.renderToElementWithId(<App />, "App");