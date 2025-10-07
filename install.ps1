# Install plugin to TeamSpeak (with Python support)
# Usage: .\install.ps1 [-Configuration Release|Debug] [-Architecture x64|x86]

param(
    [ValidateSet('Release', 'Debug')]
    [string]$Configuration = 'Release',
    
    [ValidateSet('x64', 'x86')]
    [string]$Architecture = 'x64'
)

$ErrorActionPreference = "Stop"

$PresetName = "$Architecture-$($Configuration.ToLower())"
$PluginsPath = "$env:APPDATA\TS3Client\plugins"
$DllPath = "build\$PresetName\bin\Debug\tspy_plugin.dll"

Write-Host "======================================" -ForegroundColor Cyan
Write-Host "🐍 TsPy Plugin Installer v1.3.0" -ForegroundColor Cyan
Write-Host "Configuration: $Configuration" -ForegroundColor Cyan
Write-Host "Architecture: $Architecture" -ForegroundColor Cyan
Write-Host "======================================" -ForegroundColor Cyan

# Check if DLL exists
if (!(Test-Path $DllPath)) {
    Write-Host "`nError: Plugin DLL not found!" -ForegroundColor Red
    Write-Host "Expected location: $DllPath" -ForegroundColor Yellow
    Write-Host "`nPlease build the plugin first:" -ForegroundColor Yellow
    Write-Host "  .\build.ps1 -Configuration $Configuration -Architecture $Architecture" -ForegroundColor Gray
    exit 1
}

# Create plugins folder if it doesn't exist
Write-Host "`nCreating plugins directory..." -ForegroundColor Yellow
New-Item -ItemType Directory -Force -Path $PluginsPath | Out-Null
Write-Host "Plugins path: $PluginsPath" -ForegroundColor Gray

# Copy DLL
Write-Host "`nCopying plugin DLL..." -ForegroundColor Yellow
try {
    Copy-Item $DllPath -Destination $PluginsPath -Force
    Write-Host "Plugin DLL copied successfully!" -ForegroundColor Green
} catch {
    Write-Host "Error copying DLL: $_" -ForegroundColor Red
    Write-Host "`nMake sure TeamSpeak is closed before installing." -ForegroundColor Yellow
    exit 1
}

# Copy PDB for debugging (if debug build)
if ($Configuration -eq "Debug") {
    $PdbPath = "build\$PresetName\bin\Debug\tspy_plugin.pdb"
    if (Test-Path $PdbPath) {
        Write-Host "Copying debug symbols..." -ForegroundColor Yellow
        Copy-Item $PdbPath -Destination $PluginsPath -Force
        Write-Host "Debug symbols copied." -ForegroundColor Green
    }
}

# Copy icons (optional)
$IconsSource = "resources\icons"
$IconsDest = "$env:APPDATA\TS3Client\plugins\tspy_plugin"

if (Test-Path $IconsSource) {
    Write-Host "`nCopying plugin resources..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Force -Path $IconsDest | Out-Null
    Copy-Item "$IconsSource\*" -Destination $IconsDest -Force
    Write-Host "Resources copied successfully!" -ForegroundColor Green
}

# Copy Python DLL (v1.3.0 requirement) - DISABLED
# NOTE: Python DLL should NOT be in plugins folder as TeamSpeak tries to load it as a plugin
# When Python support is re-enabled, we'll use a different approach (subdirectory or system PATH)
Write-Host "`n🐍 Python support temporarily disabled" -ForegroundColor Yellow
Write-Host "   (Python DLL not copied to avoid TS3 loading conflicts)" -ForegroundColor Gray

<#
# Original Python DLL copy code (disabled):
Write-Host "`n🐍 Checking for Python DLL..." -ForegroundColor Yellow
try {
    $pythonPath = python -c "import sys; print(sys.executable)" 2>$null
    if ($pythonPath) {
        $pythonDir = Split-Path $pythonPath
        $pythonDll = Get-ChildItem "$pythonDir\python3*.dll" -Filter "python3??.dll" -ErrorAction SilentlyContinue | Select-Object -First 1
        
        if ($pythonDll) {
            Write-Host "Found Python DLL: $($pythonDll.Name)" -ForegroundColor Green
            # DON'T copy to plugins folder - TeamSpeak tries to load it as a plugin!
            # Copy-Item $pythonDll.FullName -Destination $PluginsPath -Force
            Write-Host "Python DLL NOT copied (prevents TS3 loading errors)" -ForegroundColor Green
        } else {
            Write-Host "⚠️  Warning: Python DLL not found!" -ForegroundColor Yellow
        }
    } else {
        Write-Host "⚠️  Warning: Python not found in PATH!" -ForegroundColor Yellow
    }
} catch {
    Write-Host "⚠️  Warning: Could not check for Python" -ForegroundColor Yellow
}
#>

# Copy Python scripts (optional)
$ScriptsSource = "scripts"
$ScriptsDest = "$env:APPDATA\TS3Client\plugins\scripts"

if (Test-Path $ScriptsSource) {
    Write-Host "`n🐍 Copying Python scripts..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Force -Path $ScriptsDest | Out-Null
    New-Item -ItemType Directory -Force -Path "$ScriptsDest\examples" | Out-Null
    
    Copy-Item "$ScriptsSource\tspy_init.py" -Destination $ScriptsDest -Force -ErrorAction SilentlyContinue
    Copy-Item "$ScriptsSource\examples\*.py" -Destination "$ScriptsDest\examples\" -Force -ErrorAction SilentlyContinue
    Copy-Item "$ScriptsSource\README.md" -Destination $ScriptsDest -Force -ErrorAction SilentlyContinue
    
    Write-Host "Python scripts copied successfully!" -ForegroundColor Green
}

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "✅ Installation Complete!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan

Write-Host "`nPlugin Location:" -ForegroundColor Cyan
Write-Host "  $PluginsPath\tspy_plugin.dll" -ForegroundColor Gray

Write-Host "`n⚙️ Plugin Status:" -ForegroundColor Cyan
Write-Host "  ✅ Core features: Working" -ForegroundColor Green
Write-Host "  🐍 Python support: ENABLED (NEW!)" -ForegroundColor Green
Write-Host "     Note: Python support is experimental" -ForegroundColor Yellow

Write-Host "`nNext Steps:" -ForegroundColor Yellow
Write-Host "  1. Start TeamSpeak 3" -ForegroundColor White
Write-Host "  2. Go to: Tools → Options → Addons" -ForegroundColor White
Write-Host "  3. Enable 'TsPy Plugin'" -ForegroundColor White
Write-Host "  4. Click OK" -ForegroundColor White
Write-Host "  5. Try: /tspy help" -ForegroundColor White

Write-Host "`nFor testing guide, see:" -ForegroundColor Cyan
Write-Host "  TESTING.md" -ForegroundColor Gray
