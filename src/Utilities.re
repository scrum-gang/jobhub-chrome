/** Convert value from event to string
One of the disadvantages from reason is that you have to do this type casting */
let valueFromEvent = evt => (
                              evt
                              |> ReactEventRe.Form.target
                              |> ReactDOMRe.domElementToObj
                            )##value;

let str = ReasonReact.stringToElement;

let bool = x => Js.Boolean.to_js_boolean(x);

[@bs.deriving abstract]
type tabURL = {url: string};

[@bs.val] external createTab : tabURL => unit = "chrome.tabs.create";
