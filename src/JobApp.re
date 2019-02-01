open Chrome.Extensions;
open Chrome.Runtime;

let component = ReasonReact.statelessComponent("JobAppPage");

let make = (_children) => {
  ...component, /* spread the template's other defaults into here  */
  render: _self => <div> (ReasonReact.stringToElement("I'm in JobApp")) </div>
};