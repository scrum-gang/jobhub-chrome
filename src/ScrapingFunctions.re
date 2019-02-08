/** For checking if a URL is valid */
let urlRegex = [%bs.re "/^((?:https?:\/\/)?[^./]+(?:\.[^./]+)+(?:\/.*)?)$/"];
let postedDateRegex = [%bs.re "/(\d+)[\s]+days[\s]+ago/gim"];

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

/** Error checking with comfy optional types  */

let scriptBody = "document.body.innerText";

let validateNonNull = x =>
  switch (x) {
  | None => failwith("Value is null")
  | Some(x) => x
  };

let toStringProcess = x => Js.String.make(x);

let extractPostedDateProcess = x: string => {
  let stringBody = x |> Js.String.make;

  /* return the date x days ago in the format yyyy-MM-dd */
  let daysAgoDate(x: string) {
    let now = Js.Date.make();

    let startIsoOfset = 0;
    let lengthIsoOfset = 10;

    switch (float_of_int(-1*(int_of_string(x)))) {
    | delta => Js.Date.setDate(now, delta)
      |> Js.Date.fromFloat
      |> Js.Date.toISOString
      /* unfortunately Js.Date has no method to obtain format yyyy-MMM-dd
         so we obtain the ISO format and extract the substring of interest */
      |> Js.String.substrAtMost(~from=startIsoOfset, ~length=lengthIsoOfset);
    | exception(reason) => Js.log(reason); "";
    };
  };

  /* extract the first match from a Regex execution result */
  let extractFirstMatch = (x: Js.Re.result) => Js.Re.captures(x)[1] |> Js.Nullable.toOption;

  /* Unfortunately could not find single method in the Js.Re API to execute and obtain
     the matched strings. So it's a 2 step process: exec the Regex, then capture the match */
  switch (Js.Re.exec(stringBody, postedDateRegex)) {
  | None => "";
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
