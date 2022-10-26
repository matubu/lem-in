const std = @import("std");

pub fn main() void {
    std.debug.print("Hello {s}\n", .{"World"});
}

test "add" {
    try std.testing.expect(2 == 1 + 1);
}

test "test test failed" {
    try std.testing.expect(2 == 1);
}
