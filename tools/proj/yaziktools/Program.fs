open System
open IronPython.Hosting
open Microsoft.Scripting.Hosting

type YPlatforms 
    = Windows
    | MacOsX
    | Android
    | Ios
    | IosSimulator
    | Linux 

type YArchs
    = Armv7
    | Arm64
    | X86_64
    | I386

type YFlavour = {
    Platform : YPlatforms
    Arch : YArchs    
}

let supportedFlavours = [|
    { Platform = MacOsX ; Arch = X86_64},
    { Platform = Windows ; Arch = X86_64},
    { Platform = Linux ; Arch = X86_64}
|]

let platformDef (x:YPlatforms) = 
    match x with
    | Windows       -> "windows"
    | MacOsX        -> "macosx"
    | Android       -> "android"
    | Ios           -> "iphoneos"
    | IosSimulator  -> "iphonesimulator"
    | Linux         -> "linux"

let archDef (x:YArchs) =
    match x with
    | Armv7         -> "armv7"
    | Arm64         -> "arm64"
    | X86_64        -> "x86_64"
    | I386          -> "i386"

let flavourDef (x:YFlavour) =
    platformDef(x.Platform) + "-" + archDef(x.Arch)

[<EntryPoint>]
let main argv =
//    printfn "%s" (flavourDef{ Platform = MacOsX ; Arch = X86_64})
    let pyEngine : ScriptEngine = Python.CreateEngine()
    let pyScope : ScriptScope = pyEngine.CreateScope()
    pyEngine.ExecuteFile("buck_cxx_libraries_to_json.py",pyScope)
    0 // return an integer exit code
