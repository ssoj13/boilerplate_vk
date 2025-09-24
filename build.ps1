param(
    [string]$Config = "Release",
    [switch]$Clean
)

$buildDir = "build"

# Clean build directory if requested
if ($Clean -and (Test-Path $buildDir)) {
    Write-Host "Cleaning build directory..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $buildDir
}

# Install Conan dependencies
Write-Host "Installing Conan dependencies..." -ForegroundColor Green
conan install . --build=missing

if ($LASTEXITCODE -ne 0) {
    Write-Host "Conan install failed!" -ForegroundColor Red
    exit 1
}

# Configure with CMake using Conan preset
Write-Host "Configuring project with Conan..." -ForegroundColor Green
cmake --preset conan-default

if ($LASTEXITCODE -ne 0) {
    Write-Host "Configuration failed!" -ForegroundColor Red
    exit 1
}

# Build
Write-Host "Building project..." -ForegroundColor Green
cmake --build $buildDir --config $Config --parallel

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

Write-Host "Build completed successfully!" -ForegroundColor Green