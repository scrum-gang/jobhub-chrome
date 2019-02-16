open Chrome.Extensions;

type scrapingReturn('a) =
  | None
  | Some('a);

/** This is where all the scraping beauty happens
MAKE SURE THAT WHATEVER SCRIPT YOU'RE PASSING, IT RETURNS A VALUE,
WHICH YOU CAN THEN MANIPULATE IN PROCESSFN AND VALIDATE IN VALIDATIONFN
First: validate (return null if error)
Then: process the data how you want (make sure that the output of this function matches the input of the reducerFn, though, and of your action type)
Then: update the state of your input field with a reducer function (which is really a dispatcher) */
let scrape = (script, validationFn, processFn, reducerFn) => {
  let scriptDetails = Tabs.mkScriptDetails(~code=script, ());
  Tabs.executeScriptWithCallback(
    scriptDetails,
    result => {
      /** Type casting to option type to get around the bad bindings the original author of the
      Chrome bindings provided */
      let result = result[0] |> Obj.magic |> Js.Nullable.toOption;
      switch (result |> validationFn) {
      | item => item |> processFn |> reducerFn
      | exception (Js.Exn.Error(err)) =>
        /**Fancy stuff so that the error displays as red in the console
        and there's a default validation message if needed */
        Js.Exn.message(err)
        |> Js.Option.getWithDefault("Validation failed")
        |> Js.log
      };
    },
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
      _self => scrape(script, validationFn, processFn, reducerFn),
    ),
  render: _self =>
    <input
      _type=typeValue
      name
      placeholder
      value
      /*** Ideally, I would've passed a separate prop such as a "dispatcher" function,
           but honestly for the complexity of our app, I wouldn't bother  */
      onChange=(evt => Services.valueFromEvent(evt) |> reducerFn)
    />,
};
