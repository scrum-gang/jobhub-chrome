/** For checking if a URL is valid */
let urlRegex = [%bs.re "/^((?:https?:\/\/)?[^./]+(?:\.[^./]+)+(?:\/.*)?)$/"];

/** heuristic: the first match for "<num> day[s] ago" in top-bottom order
    tends to be the posted date of the job application. Subsequent matches
    are often posted dates for other offers being advertized.*/
let postedDateRegex = [%bs.re "/(\d+)[\s]+day[s]?[\s]+ago/im"];

let scriptUrl = "document.URL";

/** TODO: Need to find some good logic for this,
Right now it's here to demonstrate erro validation */
let scriptCompany = "document.h1";

/** Idea: gather all headers, find which one is the largest -- this one usually indicates the role for which one is applying  */
/** NOTE: MAKE SURE TO DEFINE FUNCTIONS AS VAR, SO THAT THE CODE CAN REDECLARE THE FUNCTIONS EVERY TIME THE PAGE IS RENDERED */
let scriptPosition = "  var getAreaAndText = el => ({ fontSize: parseFloat(window.getComputedStyle(el, null).getPropertyValue('font-size')), text: el.innerText });
                        var isVisible = el => el.offsetWidth > 0 || el.offsetHeight > 0;
                        var retrieveHeader = header => Array.prototype.slice.call(document.getElementsByTagName(header))
                                                      .filter(isVisible)
                                                      .map(getAreaAndText);

                        [...retrieveHeader('h1'), ...retrieveHeader('h2'), ...retrieveHeader('h3'),
                        ...retrieveHeader('h4'), ...retrieveHeader('h5'), ...retrieveHeader('h6')]
                        .filter(el => el.text)
                        .sort((a, b) => b.fontSize - a.fontSize)[0].text";

/** Error checking with comfy optional types */

let scriptBody = "document.body.innerHTML";

let validateNonNull = x =>
  switch (x) {
  | None => failwith("Value is null")
  | Some(x) => x
  };

let toStringProcess = x => Js.String.make(x);

/** return the date x days ago in the format yyyy-MM-dd */
let daysAgoDate = (x: string) => {
  let now = Js.Date.make();

  /** ISO date format is yyyy-MM-ddThh:mm:ss:msms.nnnZ, we only need the first 10 characters*/
  let startIsoOfset = 0;
  let lengthIsoOfset = 10;

  switch (Js.Date.getDate(now) -. float_of_int(int_of_string(x))) {
  | delta => Js.Date.setDate(now, delta)
    |> Js.Date.fromFloat
    |> Js.Date.toISOString
    /** unfortunately Js.Date has no method to obtain format yyyy-MMM-dd
       so we obtain the ISO format and extract the substring of interest */
    |> Js.String.substrAtMost(~from=startIsoOfset, ~length=lengthIsoOfset);
  | exception (Js.Exn.Error(err)) =>
      Js.Exn.message(err)
      |> Js.Option.getWithDefault("Unable to cast string to date")
      /** trick to be able to log while still returning a string */
      |> Js.log; "";
  };
};

let extractPostedDateProcess = x: string => {
  let stringBody = x |> Js.String.make;

  /** extract the first match from a Regex execution result */
  let extractFirstMatch = (x: Js.Re.result) => Js.Re.captures(x)[1] |> Js.Nullable.toOption;

  /** Unfortunately there is no single method in the Js.Re API to execute and obtain
     the matched strings. So it's a 2 step process: exec the Regex, then capture the match */
  switch (Js.Re.exec(stringBody, postedDateRegex)) {
  /** trick to be able to log while still returning a string */
  | None => Js.log("Unable to find posted date"); "";
  | Some(x) =>
    switch (extractFirstMatch(x)) {
    | None => "";
    | Some(x) => daysAgoDate(x);
    };
  };
};

let checkValidUrl = x => {
  let stringUrl = x |> Js.String.make;
  Js.Re.test(stringUrl, urlRegex) ? x : failwith("Invalid URL");
};

let checkValidPostedDate = x => {
  let stringUrl = x |> Js.String.make;
  Js.Re.test(stringUrl, postedDateRegex) ? x : failwith("Unable to find posted date");
};

let validateUrl = x => x |> validateNonNull |> checkValidUrl;

let validateDate = x => x |> validateNonNull |> checkValidPostedDate;
