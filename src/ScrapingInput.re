open Chrome.Extensions;

/** Convert value from event to string */
let valueFromEvent = evt : string => (
                                       evt
                                       |> ReactEventRe.Form.target
                                       |> ReactDOMRe.domElementToObj
                                     )##value;

/** This is where all the scraping beauty happens
First: validate (return null if error)
Then: process the data how you want
Then: update the state of your input field with a reducer function */
let scrape = (script, validationFn, processFn, reducerFn) => {
  let scriptDetails = Tabs.mkScriptDetails(~code=script, ());
  Tabs.executeScriptWithCallback(scriptDetails, result =>
    result[0] |> validationFn |> processFn |> reducerFn
  );
};

let component = ReasonReact.statelessComponent("ScrapingInput");

let make =
    (
      ~script,
      ~typeValue,
      ~validationFn,
      ~processFn,
      ~reducerFn,
      ~name,
      ~placeholder,
      ~value,
      _children,
    ) => {
  ...component,
  didMount: _self =>
    ReasonReact.SideEffects(
      self => scrape(script, validationFn, processFn, reducerFn),
    ),
  render: _self =>
    <input
      _type=typeValue
      name
      placeholder
      value
      tabIndex=1
      onChange=(evt => valueFromEvent(evt) |> reducerFn)
    />,
};