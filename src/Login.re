open Chrome.Extensions;
open Chrome.Runtime;

let component = ReasonReact.statelessComponent("Login");

let make = (_children) => {
  ...component, 
  render: _self => <div> (ReasonReact.stringToElement("I'm in Login")) </div>
};