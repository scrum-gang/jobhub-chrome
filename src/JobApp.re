let component = ReasonReact.statelessComponent("JobApp");

let make = (~submitHandler, ~signOutHandler, _children) => {
  ...component, /* spread the template's other defaults into here  */
  render: _self =>
    <div>
      <form>
        <input name="company" placeholder="company" tabIndex=1 />
        <input name="position" placeholder="position" tabIndex=2 />
        <input _type="url" name="url" placeholder="url" tabIndex=3 />
        <div className="form-horizontal-separator">
          <label>(ReasonReact.stringToElement("Date posted"))</label>
            <input _type="date"  name="dateposted" tabIndex=4 />
        </div>
        <div className="form-horizontal-separator">
          <label>(ReasonReact.stringToElement("Deadline"))</label>
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
    </div>,
};
