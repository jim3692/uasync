{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs =
    { self, nixpkgs, ... }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
      ];

      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
      pkgsFor = system: nixpkgs.legacyPackages.${system};

      depsFor =
        system: with (pkgsFor system); [
          clang
          gdb
          gnumake
        ];

      cxxFlagsFor =
        system:
        with (pkgsFor system);
        builtins.concatStringsSep " " [
          (lib.removeSuffix "\n" (builtins.readFile "${clang}/nix-support/cc-cflags"))
          (lib.removeSuffix "\n" (builtins.readFile "${clang}/nix-support/libc-cflags"))
          (lib.removeSuffix "\n" (builtins.readFile "${clang}/nix-support/libcxx-cxxflags"))
        ];
    in
    {
      devShells = forAllSystems (system: {
        default =
          with (pkgsFor system);
          mkShell {
            buildInputs = depsFor system;
            CXXFLAGS = cxxFlagsFor system;
            shellHook = "export CXXFLAGS+=$NIX_CFLAGS_COMPILE";
          };
      });

      packages = forAllSystems (system: {
        default =
          with (pkgsFor system);
          runCommand "test" { } ''
            export PATH=${lib.makeBinPath (depsFor system)}:$PATH
            export CXXFLAGS="${cxxFlagsFor system}"
            mkdir -p $out/bin
            ( cd "${self}" ; make build OUT_DIR=$out/bin )
          '';
      });
    };
}
