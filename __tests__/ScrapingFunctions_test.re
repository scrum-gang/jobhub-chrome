open Jest;
open Expect;
open ScrapingFunctions;

/** TODO: figure out how to test toThrow (failwith) */

describe("Validation Functions", () => {
  /** TODO: add invalid cases */
  describe("validateNonNull", () => {
    test("with something", () => expect(validateNonNull(Js.Option.some("something"))) |> toBe("something"));
  });
});

describe("Process Functions", () => {
  describe("toStringProcess", () => {
    test("number", () => expect(toStringProcess(3.5)) |> toBe("3.5"));
    test("array", () => expect(toStringProcess([|1,2,3|])) |> toBe("1,2,3"));
    test("none", () => expect(toStringProcess(None)) |> toBe("0"));
  });
});

describe("Date Functions", () => {
  /** TODO: add invalid cases */
  describe("formatDate", () => {
    let pastDate = Js.Date.makeWithYMD(~year=1970.0, ~month=0.0, ~date=1.0, ());
    let futureDate = Js.Date.makeWithYMD(~year=2020.0, ~month=10.0, ~date=13.0, ());

    test("past date", () => expect(formatDate(pastDate |> Js.Date.getTime)) |> toBe("1970-01-01"));
    test("future date", () => expect(formatDate(futureDate |> Js.Date.getTime)) |> toBe("2020-11-13"));
  });

  /** TODO: add invalid cases */
  describe("daysAgoDate", () => {
    let now = Js.Date.make();
    let today = now |> Js.Date.getTime;
    /** must pass a new Js.Date.t everytime because setDate changes the passed date */
    let minus5DaysAgo = Js.Date.setDate(Js.Date.make(), Js.Date.getDate(now) -. 5.0);
    let plus10DaysAgo = Js.Date.setDate(Js.Date.make(), Js.Date.getDate(now) +. 10.0);
    let minus160DaysAgo = Js.Date.setDate(Js.Date.make(), Js.Date.getDate(now) -. 160.0);

    test("0 days ago", () => expect(daysAgoDate("0")) |> toBe(today |> formatDate));
    test("5 days in the future", () => expect(daysAgoDate("5")) |> toBe(minus5DaysAgo |> formatDate));
    test("160 days ago", () => expect(daysAgoDate("160")) |> toBe(minus160DaysAgo |> formatDate));
    test("5 days in the future", () => expect(daysAgoDate("-10")) |> toBe(plus10DaysAgo |> formatDate));
  });

  describe("extractPostedDateProcess", () => {
    let stringWithDate = "Lorem ipsum dolor sit 9 days ago, consectetur";
    let stringWithoutDate = "Lorem ipsum dolor sit, consectetur";

    test("contains date", () => {
      expect(extractPostedDateProcess(stringWithDate) |> Js.String.length) |> toEqual(10);
    });
    test("does not contain date", () => {
      expect(extractPostedDateProcess(stringWithoutDate) |> Js.String.length) |> toEqual(0);
    });
  });
});
