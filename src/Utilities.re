/** Convert value from event to string
One of the disadvantages from reason is that you have to do this type casting */
let valueFromEvent = evt => (
                              evt
                              |> ReactEventRe.Form.target
                              |> ReactDOMRe.domElementToObj
                            )##value;
