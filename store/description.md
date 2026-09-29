# wxl-combo-points

A small WarcraftXL extension for the WotLK 3.3.5a (12340) client.

## What it does

The stock client contains a class restriction in the native combo-point path. WotLK-Extensions removes that restriction by changing the opcode at client VA `0x611707` from `0x74` (`JE short`) to `0xEB` (`JMP short`).

This extension applies the same one-byte runtime patch.

## Safety

The extension:

- accepts only WXL API version 1 and client build 12340;
- resolves the patch through the executable image base plus RVA;
- patches only when the expected original byte `0x74` is present;
- treats `0xEB` as already patched;
- refuses to overwrite an unexpected byte;
- restores the original memory protection after the write.

No AIO/Lua replacement system is used. The client continues using its native combo-point implementation.

Do not load this together with another extension that applies the same combo-point byte patch.
