open Jest;

describe("Vanilla Jest Test", () => {
  open Expect;

  test("toBe", () => expect(1+2) |> toBe(3));
  test("NotToBe", () => expect(1+2) |> not_ |> toBe(4));
});

