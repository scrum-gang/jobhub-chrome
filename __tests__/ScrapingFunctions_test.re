open Jest;
open Expect;
open ScrapingFunctions;

describe("Validation Functions", () => {
  /** TODO: figure out how to test toThrow (failwith) */
  describe("validateNonNull", () => {
    test("to be something", () => expect(validateNonNull(Js.Option.some("something"))) |> toBe("something"));
  });
});


describe("Process Functions", () => {
  describe("toStringProcess", () => {
    test("number", () => expect(toStringProcess(3.5)) |> toBe("3.5"));
    test("array", () => expect(toStringProcess([|1,2,3|])) |> toBe("1,2,3"));
    test("none", () => expect(toStringProcess(None)) |> toBe("0"));
  });
});
