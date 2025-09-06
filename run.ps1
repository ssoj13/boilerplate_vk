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

# Run the application from the build directory to ensure correct working directory
Write-Host "Running Vulkan App..." -ForegroundColor Green
Push-Location "$buildDir/$Config"
try {
    & ".\$exeName"
} finally {
    Pop-Location
}

Write-Host "Application exited." -ForegroundColor Green