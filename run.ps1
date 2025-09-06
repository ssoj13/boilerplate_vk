param(
    [string]$Config = "Release"
)

$buildDir = "build"
$exeName = "VulkanApp.exe"

# Check if the executable exists
$exePath = "$buildDir/$Config/$exeName"
if (!(Test-Path $exePath)) {
    Write-Host "Executable not found at $exePath" -ForegroundColor Red
    Write-Host "Please build the project first using build.ps1" -ForegroundColor Yellow
    exit 1
}

# Run the application
Write-Host "Running Vulkan App..." -ForegroundColor Green
& $exePath

Write-Host "Application exited." -ForegroundColor Green