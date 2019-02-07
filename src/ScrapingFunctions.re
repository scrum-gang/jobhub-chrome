/** For checking if a URL is valid */
let regexConstant = [%bs.re
  "/^((?:https?:\/\/)?[^./]+(?:\.[^./]+)+(?:\/.*)?)$/"
];

let scriptUrl = "document.URL";

/** TODO: Need to find some good logic for this,
Right now it's here to demonstrate erro validation */
let scriptCompany = "document.h1";

/** Idea: gather all headers, find which one is the largest -- this one usually indicates the role for which one is applying  */
/** NOTE: MAKE SURE TO DEFINE FUNCTIONS AS VAR, SO THAT THE CODE CAN REDECLARE THE FUNCTIONS EVERY TIME THE PAGE IS RENDERED */
let scriptPosition = "  var getAreaAndText = el => ({ area: el.getBoundingClientRect().width * el.getBoundingClientRect().height, text: el.innerText });
                        var retrieveHeader = header => Array.prototype.slice.call(document.getElementsByTagName(header)).map(getAreaAndText);

                        [...retrieveHeader('h1'), ...retrieveHeader('h2'), ...retrieveHeader('h3'),
                        ...retrieveHeader('h4'), ...retrieveHeader('h5'), ...retrieveHeader('h6')]
                        .filter(Boolean).sort((a, b) => b.area - a.area)[0].text";

/** Error checking with comfy optional types  */
let validateNonNull = x =>
  switch (x) {
  | None => failwith("Value is null")
  | Some(x) => x
  };

let toStringProcess = x => Js.String.make(x);

let validateUrl = x => {
  let stringUrl = Js.String.make(x |> validateNonNull);
  Js.Re.test(stringUrl, regexConstant) ? x : failwith("Invalid URL");
};