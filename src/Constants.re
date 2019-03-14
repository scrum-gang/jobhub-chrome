/** For checking if a URL is valid */
let urlRegex = [%bs.re "/^((?:https?:\/\/)?[^./]+(?:\.[^./]+)+(?:\/.*)?)$/"];

/** heuristic: the first match for "<num> day[s] ago" in top-bottom order
    tends to be the posted date of the job application. Subsequent matches
    are often posted dates for other offers being advertized.*/
let postedDateRegex = [%bs.re "/(\d+)[\s]+day[s]?[\s]+ago/im"];

let linkedinCDNURL = "https://static.licdn.com";

let linkedinCompanyNameLowerCase = "linkedin";

let authUrl = "https://jobhub-authentication.herokuapp.com";

let jobAppUrl = "https://scrum-gang-job-applications.herokuapp.com";

let resumesUrl = "https://resume-revision.herokuapp.com";

let registerUrl = "https://quizzical-mccarthy-0289ae.netlify.com/register";
