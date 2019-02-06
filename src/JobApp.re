type state = {url: string};

type action =
  | UpdateUrl(string);

let reducer = (action, _state) =>
  switch (action) {
  | UpdateUrl(value) => ReasonReact.Update({url: value})
  };

let id = x => x;

let component = ReasonReact.reducerComponent("JobApp");

let make = (~submitHandler, ~signOutHandler, _children) => {
  ...component, /* spread the template's other defaults into here  */
  reducer,
  initialState: () => {url: ""},
  render: _self => {
    let sendUrl = x => _self.send(UpdateUrl(x));
    <div>
      <form>
        <input name="company" placeholder="company" tabIndex=2 />
        <input name="position" placeholder="position" tabIndex=2 />
        <ScrapingInput
          script="document.URL"
          typeValue="url"
          validationFn=id
          processFn=(x => Js.String.make(x))
          reducerFn=sendUrl
          name="url"
          placeholder="url"
          value=_self.state.url
        />
        <div className="form-horizontal-separator">
          <label> (ReasonReact.stringToElement("Date posted")) </label>
          <input _type="date" name="dateposted" tabIndex=4 />
        </div>
        <div className="form-horizontal-separator">
          <label> (ReasonReact.stringToElement("Deadline")) </label>
          <input _type="date" name="deadline" tabIndex=5 />
        </div>
        <div className="form-horizontal-separator">
          <label>
            <input _type="radio" name="status" value="applied" tabIndex=6 />
            (ReasonReact.stringToElement("Applied"))
          </label>
          <label>
            <input _type="radio" name="status" value="toApply" tabIndex=7 />
            (ReasonReact.stringToElement("To apply"))
          </label>
        </div>
        <button className="btn submit-btn" onClick=submitHandler tabIndex=8>
          (ReasonReact.stringToElement("Submit"))
        </button>
        <span className="form-vertical-separator">
          <p className="form-vertical-separator-txt">
            (ReasonReact.stringToElement("or"))
          </p>
        </span>
        <button className="btn signout-btn" onClick=signOutHandler tabIndex=8>
          (ReasonReact.stringToElement("Sign Out"))
        </button>
      </form>
    </div>;
  },
};