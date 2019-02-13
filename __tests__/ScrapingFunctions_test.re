open Jest;
open Expect;
open ScrapingFunctions;

/** TODO: figure out how to test toThrow (failwith) */

describe("Validation Functions", () => {
  describe("validateNonNull", () => {
    /** TODO: add invalid cases */
    let someValue = Js.Option.some("something");
    test("with something", () => {
      validateNonNull(someValue)
      |> expect
      |> toBe("something")
    });
  });
});

describe("Process Functions", () => {
  describe("toStringProcess", () => {
    test("number", () => {
      toStringProcess(3.5)
      |> expect
      |> toBe("3.5")
    });
    test("array", () => {
      toStringProcess([|1,2,3|])
      |> expect
      |> toBe("1,2,3")
    });
    test("none", () => {
      toStringProcess(None)
      |> expect
      |> toBe("0")
    });
  });
});

describe("Date Functions", () => {
  describe("formatDate", () => {
    /** TODO: add invalid cases */
    let pastDate = Js.Date.makeWithYMD(~year=1970.0, ~month=0.0, ~date=1.0, ());
    let futureDate = Js.Date.makeWithYMD(~year=2020.0, ~month=10.0, ~date=13.0, ());
    let floatPastDate = pastDate |> Js.Date.getTime;
    let floatFutureDate = futureDate |> Js.Date.getTime;

    test("future date", () => {
      formatDate(floatPastDate)
      |> expect
      |> toBe("1970-01-01")
    });
    test("past date", () => {
      formatDate(floatFutureDate)
      |> expect
      |> toBe("2020-11-13")
    });
  });

  describe("daysAgoDate", () => {
    /** TODO: add invalid cases */
    let now = Js.Date.make();
    let today = now |> Js.Date.getTime;
    /** must pass a new Js.Date.t everytime because setDate changes the passed date */
    let minus5DaysAgo = Js.Date.setDate(Js.Date.make(), Js.Date.getDate(now) -. 5.0);
    let plus10DaysAgo = Js.Date.setDate(Js.Date.make(), Js.Date.getDate(now) +. 10.0);
    let minus160DaysAgo = Js.Date.setDate(Js.Date.make(), Js.Date.getDate(now) -. 160.0);

    test("0 days ago", () => {
      daysAgoDate("0")
      |> expect
      |> toBe(today |> formatDate)
    });
    test("5 days in the future", () => {
      daysAgoDate("5")
      |> expect
      |> toBe(minus5DaysAgo |> formatDate)
    });
    test("160 days ago", () => {
      daysAgoDate("160")
      |> expect
      |> toBe(minus160DaysAgo |> formatDate)
    });
    test("5 days in the future", () => {
      daysAgoDate("-10")
      |> expect
      |> toBe(plus10DaysAgo |> formatDate)
    });
  });

  let stringWithDate = "Lorem ipsum dolor sit 9 days ago, consectetur";
  let stringWithoutDate = "Lorem ipsum dolor sit, consectetur";

  describe("extractPostedDateProcess", () => {
    test("contains date", () => {
      extractPostedDateProcess(stringWithDate)
      |> Js.String.length
      |> expect
      |> toEqual(10)
    });
    test("does not contain date", () => {
      extractPostedDateProcess(stringWithoutDate)
      |> Js.String.length
      |> expect
      |> toEqual(0);
    });
  });

  describe("validateDate", () => {
    let optionWithDate = Js.Option.some(stringWithDate);
    let lengthString = Js.String.length(stringWithDate);

    test("valid date", () => {
      validateDate(optionWithDate)
      |> Js.String.length
      |> expect
      |> toEqual(lengthString)
    });
  });
});
