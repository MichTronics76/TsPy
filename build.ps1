# Build script for TsPy Plugin
# Usage: .\build.ps1 [-Configuration Release|Debug] [-Architecture x64|x86] [-Clean]

param(
    [ValidateSet('Release', 'Debug')]
    [string]$Configuration = 'Release',
    
    [ValidateSet('x64', 'x86')]
    [string]$Architecture = 'x64',
    
    [switch]$Clean
)

$ErrorActionPreference = "Stop"

# Determine preset name
$PresetName = "$Architecture-$($Configuration.ToLower())"

Write-Host "======================================" -ForegroundColor Cyan
Write-Host "Building TsPy Plugin" -ForegroundColor Cyan
Write-Host "Configuration: $Configuration" -ForegroundColor Cyan
Write-Host "Architecture: $Architecture" -ForegroundColor Cyan
Write-Host "Preset: $PresetName" -ForegroundColor Cyan
Write-Host "======================================" -ForegroundColor Cyan

# Clean if requested
if ($Clean) {
    Write-Host "`nCleaning build directory..." -ForegroundColor Yellow
    if (Test-Path "build") {
        Remove-Item -Path "build" -Recurse -Force
        Write-Host "Build directory cleaned." -ForegroundColor Green
    }
}

# Configure
Write-Host "`nConfiguring CMake..." -ForegroundColor Yellow
try {
    cmake --preset $PresetName
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed"
    }
    Write-Host "Configuration successful." -ForegroundColor Green
} catch {
    Write-Host "Error during configuration: $_" -ForegroundColor Red
    exit 1
}

# Build
Write-Host "`nBuilding project..." -ForegroundColor Yellow
try {
    cmake --build --preset $PresetName
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed"
    }
    Write-Host "Build successful!" -ForegroundColor Green
} catch {
    Write-Host "Error during build: $_" -ForegroundColor Red
    exit 1
}

# Display output location
$OutputDir = "build\$PresetName\bin"
if (Test-Path $OutputDir) {
    Write-Host "`n======================================" -ForegroundColor Cyan
    Write-Host "Build Complete!" -ForegroundColor Green
    Write-Host "Output directory: $OutputDir" -ForegroundColor Cyan
    Write-Host "`nOutput files:" -ForegroundColor Cyan
    Get-ChildItem $OutputDir -Recurse -File | ForEach-Object {
        Write-Host "  - $($_.FullName)" -ForegroundColor Gray
    }
    Write-Host "======================================" -ForegroundColor Cyan
} else {
    Write-Host "`nWarning: Output directory not found: $OutputDir" -ForegroundColor Yellow
}

Write-Host "`nTo install the plugin, copy tspy_plugin.dll to:" -ForegroundColor Yellow
Write-Host "  %APPDATA%\TS3Client\plugins\" -ForegroundColor Gray
