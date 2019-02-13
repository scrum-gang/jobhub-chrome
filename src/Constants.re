/** For checking if a URL is valid */
let urlRegex = [%bs.re "/^((?:https?:\/\/)?[^./]+(?:\.[^./]+)+(?:\/.*)?)$/"];

/** heuristic: the first match for "<num> day[s] ago" in top-bottom order
    tends to be the posted date of the job application. Subsequent matches
    are often posted dates for other offers being advertized.*/
let postedDateRegex = [%bs.re "/(\d+)[\s]+day[s]?[\s]+ago/im"];

let linkedinCDNURL = "https://static.licdn.com";

let linkedinCompanyNameLowerCase = "linkedin";
