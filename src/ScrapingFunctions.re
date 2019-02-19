let scriptUrl = "document.URL";

let scriptHtmlBody = "document.body.innerHTML";

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
let validateNonNull = x =>
  switch (x) {
  | None => failwith("Value is null")
  | Some(x) => x
  };

/** Note: toStringProcess(None) => "0" */
let toStringProcess = x => Js.String.make(x);

/** formats a float date to a yyyy-MM-dd string */
let formatDate = date =>
  Js.Date.fromFloat(date)
  |> Js.Date.toISOString
  |> {
    /** ISO date format is yyyy-MM-ddThh:mm:ss:msms.nnnZ, we only need the first 10 characters*/
    let startIsoOfset = 0;
    let lengthIsoOfset = 10;
    Js.String.substrAtMost(~from=startIsoOfset, ~length=lengthIsoOfset);
  };

/** return the date x days ago in the format yyyy-MM-dd */
let daysAgoDate = x : string => {
  let now = Js.Date.make();
  let delta = Js.Date.getDate(now) -. float_of_int(int_of_string(x));
  Js.Date.setDate(now, delta) |> formatDate;
};

let extractPostedDateProcess = x : string => {
  let stringBody = x |> Js.String.make;
  let result = Js.String.match(Constants.postedDateRegex, stringBody);
  switch (result) {
  | None => ""
  | Some(match) => daysAgoDate(match[1])
  };
};

/**
Heuristic: check how many times the name of a company appears.
The company name with highest frequency is the company we're looking for.

- Potential improvements: cache the company name given the URL
- Make a legit getSubstringOccurences method
- Checker --> if I'm on linkedin, don't use me as #1 result
 */
let extractCompaniesProcess = (companies: array(string), body) => {
  let stringBody = body |> Js.String.toLowerCase;
  /** Particular check for Linkedin (could've converted into its own type) */
  let linkedinCheck =
    Js.String.match(Js.Re.fromString(Constants.linkedinCDNURL), stringBody)
    != None;
  /**Skip this company if the custom check for the company doesn't pass */
  let companyCheckers = company =>
    switch (company |> Js.String.toLocaleLowerCase) {
    | x when x == Constants.linkedinCompanyNameLowerCase && linkedinCheck =>
      false
    | _ => true
    };
  /**
   Brute force method: check how many times a company name appears in a string
   */
  let reducer = (acc, x) => {
    let result =
      /** A bit sketch that it's done this way, but I'm not going to write a functional method to get substring occurences and I can't
    find a built-in one; this works just as well nonetheless*/
      Js.String.splitByRe(
        Js.Re.fromString(String.lowercase(x)),
        stringBody,
      );
    switch (result) {
    | value when Array.length(value) != 1 && companyCheckers(x) => [
        (x, Array.length(value)),
        ...acc,
      ]
    | _ => acc
    };
  };
  companies
  |> Array.fold_left(reducer, [])
  |> List.sort(((_, a), (_, b)) => b - a)
  |> List.hd
  |> (((a, _)) => a);
};

let checkValidUrl = x => {
  let stringUrl = x |> Js.String.make;
  Js.Re.test(stringUrl, Constants.urlRegex) ? x : failwith("Invalid URL");
};

let checkValidPostedDate = x : string => {
  let stringUrl = x |> Js.String.make;
  Js.Re.test(stringUrl, Constants.postedDateRegex) ?
    x : failwith("Unable to find posted date");
};

/** if the string contains a valide url it will return the input string
    if the string doesn't contain a valid url it raises a failure */
let validateUrl = x : string => x |> validateNonNull |> checkValidUrl;

/** if the string contains a valide date in the form "<num> days ago" it will return the input string
    if the string doesn't contain a valid date it raises a failure */
let validateDate = x : string => x |> validateNonNull |> checkValidPostedDate;
