open Chrome.Extensions;

/** Convert value from event to string
One of the disadvantages from reason is that you have to do this type casting */
let valueFromEvent = evt => (
                              evt
                              |> ReactEventRe.Form.target
                              |> ReactDOMRe.domElementToObj
                            )##value;

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
    /** Cannot make "result" a nullable because of the way the author of the bindings defined his return types,
    don't want to mess with his libraries
    This sacrifices proper validation with Option types or even monadic error handling,
    I could probably make this return a nullable, but that's too much trouble */
    result => {
      let result = result[0];
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
      tabIndex=1
      /*** Ideally, I would've passed a separate prop such as a "dispatcher" function,
           but honestly for the complexity of our app, I wouldn't bother  */
      onChange=(evt => valueFromEvent(evt) |> reducerFn)
    />,
};