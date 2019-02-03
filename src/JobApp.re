let component = ReasonReact.statelessComponent("JobApp");

let make = (~submitHandler, ~signOutHandler, _children) => {
  ...component, /* spread the template's other defaults into here  */
  render: _self =>
    <div>
      <form>
        /* TODO: can't add type_="". not sure why not. */
        <input name="company" placeholder="company" tabIndex=1 />
        <input name="position" placeholder="position" tabIndex=2 />
        <input name="date posted" placeholder="date posted" tabIndex=3 />
        <input name="deadline" placeholder="deadline" tabIndex=4 />
        <input name="status" placeholder="status" tabIndex=5 />
        <input name="url" placeholder="url" tabIndex=6 />
        <button className="btn submit-btn" onClick=submitHandler tabIndex=7>
          (ReasonReact.stringToElement("Submit"))
        </button>

        <span className="form-separator">
          <p className="form-separator-txt">
            (ReasonReact.stringToElement("or"))
          </p>
        </span>

        <button className="btn signout-btn" onClick=signOutHandler tabIndex=8>
          (ReasonReact.stringToElement("Sign Out"))
        </button>
      </form>
    </div>,
};
