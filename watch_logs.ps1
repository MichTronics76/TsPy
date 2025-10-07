# Watch TeamSpeak logs for TsPy plugin activity
$logPath = "$env:APPDATA\TS3Client\logs"

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "📊 TeamSpeak Log Monitor" -ForegroundColor Cyan
Write-Host "========================================`n" -ForegroundColor Cyan

Write-Host "Waiting for new TeamSpeak log file..." -ForegroundColor Yellow
Write-Host "(Start TeamSpeak now)`n" -ForegroundColor Gray

$lastFile = $null
while ($true) {
    $latestLog = Get-ChildItem $logPath -Filter "*.log" | Sort-Object LastWriteTime -Descending | Select-Object -First 1
    
    if ($latestLog -and ($lastFile -eq $null -or $latestLog.FullName -ne $lastFile)) {
        $lastFile = $latestLog.FullName
        Write-Host "`n📄 New log file detected: $($latestLog.Name)" -ForegroundColor Green
        Write-Host "Watching for TsPy activity...`n" -ForegroundColor Gray
        
        # Show last 50 lines initially
        Get-Content $lastFile -Tail 50 | Where-Object { $_ -match "TsPy|tspy|python|Python" } | ForEach-Object {
            Write-Host $_ -ForegroundColor White
        }
        
        # Follow the log file
        Get-Content $lastFile -Wait -Tail 0 | Where-Object { $_ -match "TsPy|tspy|python|Python|command" } | ForEach-Object {
            if ($_ -match "ERROR") {
                Write-Host $_ -ForegroundColor Red
            } elseif ($_ -match "WARNING") {
                Write-Host $_ -ForegroundColor Yellow
            } elseif ($_ -match "Python engine initialized successfully") {
                Write-Host $_ -ForegroundColor Green
            } elseif ($_ -match "temporarily disabled") {
                Write-Host $_ -ForegroundColor Red
            } else {
                Write-Host $_ -ForegroundColor Cyan
            }
        }
    }
    
    Start-Sleep -Milliseconds 500
}
