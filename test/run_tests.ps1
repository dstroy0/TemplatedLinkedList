# run_tests.ps1 - Build and run the TemplatedLinkedList test suite
# Usage: pwsh ./run_tests.ps1
#        pwsh ./run_tests.ps1 -Clean    (remove compiled binary before building)

param (
    [switch]$Clean
)

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$TestSrc   = Join-Path $ScriptDir "test_main.cpp"
$LibSrc    = Join-Path $ScriptDir ".." "src"
$OutBin    = Join-Path $ScriptDir "test_bin.exe"

# ─── Locate g++ ──────────────────────────────────────────────────────────────
$Gpp = $null

# Check PATH first
foreach ($name in @("g++", "g++.exe")) {
    $found = Get-Command $name -ErrorAction SilentlyContinue
    if ($found) {
        $Gpp = $found.Source
        break
    }
}

# Common MinGW / MSYS2 install locations on Windows
if (-not $Gpp) {
    $SearchPaths = @(
        "C:\msys64\mingw64\bin\g++.exe",
        "C:\msys64\ucrt64\bin\g++.exe",
        "C:\mingw64\bin\g++.exe",
        "C:\MinGW\bin\g++.exe",
        "C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe"
    )
    foreach ($path in $SearchPaths) {
        if (Test-Path $path) {
            $Gpp = $path
            break
        }
    }
}

if (-not $Gpp) {
    Write-Error @"
g++ not found.
Install MinGW-w64 (https://www.mingw-w64.org/) or MSYS2 (https://www.msys2.org/)
and ensure g++ is on your PATH, then re-run this script.
"@
    exit 1
}

Write-Host "Compiler : $($Gpp)"
Write-Host "Version  : $(& $Gpp --version | Select-Object -First 1)"
Write-Host ""

# ─── Optional clean ──────────────────────────────────────────────────────────
if ($Clean -and (Test-Path $OutBin)) {
    Remove-Item -Force $OutBin
    Write-Host "Removed existing binary."
}

# ─── Compile ─────────────────────────────────────────────────────────────────
Write-Host "Compiling $TestSrc ..."

& $Gpp `
    -std=c++11 -Wall -Wextra -Wpedantic `
    -I"$LibSrc" `
    -I"$ScriptDir" `
    -o "$OutBin" `
    "$TestSrc"

if ($LASTEXITCODE -ne 0) {
    Write-Error "Compilation failed (exit $LASTEXITCODE)."
    exit 1
}

Write-Host "Compilation succeeded.`n"

# ─── Run ─────────────────────────────────────────────────────────────────────
Write-Host "Running tests ..."
Write-Host ""

& "$OutBin"
$ExitCode = $LASTEXITCODE

# ─── Cleanup ─────────────────────────────────────────────────────────────────
if (Test-Path $OutBin) {
    Remove-Item -Force $OutBin
}

Write-Host ""
if ($ExitCode -eq 0) {
    Write-Host "Result: PASSED" -ForegroundColor Green
} else {
    Write-Host "Result: FAILED (exit $ExitCode)" -ForegroundColor Red
}

exit $ExitCode
