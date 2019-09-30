// |jit-test| skip-if: !wasmReftypesEnabled()

load(libdir + "wasm-binary.js");

const v2vSig = {args:[], ret:VoidCode};
const v2vSigSection = sigSection([v2vSig]);

function checkInvalid(body, errorMessage) {
    assertErrorMessage(() => new WebAssembly.Module(
<<<<<<< HEAD
        moduleWithSections([gcFeatureOptInSection(2), v2vSigSection, declSection([0]), bodySection([body])])),
||||||| merged common ancestors
        moduleWithSections([gcFeatureOptInSection(1), v2vSigSection, declSection([0]), bodySection([body])])),
=======
        moduleWithSections([gcFeatureOptInSection(3), v2vSigSection, declSection([0]), bodySection([body])])),
>>>>>>> upstream-releases
                       WebAssembly.CompileError,
                       errorMessage);
}

const invalidRefBlockType = funcBody({locals:[], body:[
    BlockCode,
    RefCode,
    0x42,
    EndCode,
]});
checkInvalid(invalidRefBlockType, /invalid inline block type/);

const invalidTooBigRefType = funcBody({locals:[], body:[
    BlockCode,
    RefCode,
    varU32(1000000),
    EndCode,
]});
checkInvalid(invalidTooBigRefType, /invalid inline block type/);
