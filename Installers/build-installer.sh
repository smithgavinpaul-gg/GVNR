#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_DIR/build"
INSTALLER_DIR="$PROJECT_DIR/Installers"
VERSION="1.0.0"

echo "=== The GVNR Installer Builder ==="
echo ""

# Build
echo "[1/5] Configuring build..."
cd "$PROJECT_DIR"
cmake -B "$BUILD_DIR" -G Ninja -DCMAKE_BUILD_TYPE=Release

echo "[2/5] Building plugin..."
cmake --build "$BUILD_DIR"

# Paths to built plugins
VST3_PATH="$BUILD_DIR/TheGVNR_artefacts/Release/VST3/The GVNR.vst3"
AU_PATH="$BUILD_DIR/TheGVNR_artefacts/Release/AU/The GVNR.component"
STANDALONE_PATH="$BUILD_DIR/TheGVNR_artefacts/Release/Standalone/The GVNR.app"

# Create staging directories
STAGING="$INSTALLER_DIR/staging"
rm -rf "$STAGING"
mkdir -p "$STAGING/vst3"
mkdir -p "$STAGING/au"
mkdir -p "$STAGING/app"

cp -R "$VST3_PATH" "$STAGING/vst3/"
cp -R "$AU_PATH" "$STAGING/au/"
cp -R "$STANDALONE_PATH" "$STAGING/app/"

# Build component packages
echo "[3/5] Creating component packages..."

pkgbuild --root "$STAGING/vst3" \
         --install-location "/Library/Audio/Plug-Ins/VST3" \
         --identifier "com.gvnraudio.thegvnr.vst3" \
         --version "$VERSION" \
         "$INSTALLER_DIR/TheGVNR-VST3.pkg"

pkgbuild --root "$STAGING/au" \
         --install-location "/Library/Audio/Plug-Ins/Components" \
         --identifier "com.gvnraudio.thegvnr.au" \
         --version "$VERSION" \
         "$INSTALLER_DIR/TheGVNR-AU.pkg"

pkgbuild --root "$STAGING/app" \
         --install-location "/Applications" \
         --identifier "com.gvnraudio.thegvnr.app" \
         --version "$VERSION" \
         "$INSTALLER_DIR/TheGVNR-App.pkg"

# Create distribution XML
echo "[4/5] Creating distribution..."
cat > "$INSTALLER_DIR/distribution.xml" << 'DISTXML'
<?xml version="1.0" encoding="utf-8" standalone="no"?>
<installer-gui-script minSpecVersion="2">
    <title>The GVNR</title>
    <welcome file="welcome.txt"/>
    <options customize="allow" require-scripts="false"/>
    <choices-outline>
        <line choice="vst3"/>
        <line choice="au"/>
        <line choice="app"/>
    </choices-outline>
    <choice id="vst3" title="VST3 Plugin" description="Install The GVNR VST3 for FL Studio and Ableton Live">
        <pkg-ref id="com.gvnraudio.thegvnr.vst3"/>
    </choice>
    <choice id="au" title="Audio Unit Plugin" description="Install The GVNR AU for Logic Pro X">
        <pkg-ref id="com.gvnraudio.thegvnr.au"/>
    </choice>
    <choice id="app" title="Standalone Application" description="Install The GVNR standalone app">
        <pkg-ref id="com.gvnraudio.thegvnr.app"/>
    </choice>
    <pkg-ref id="com.gvnraudio.thegvnr.vst3" version="1.0.0">TheGVNR-VST3.pkg</pkg-ref>
    <pkg-ref id="com.gvnraudio.thegvnr.au" version="1.0.0">TheGVNR-AU.pkg</pkg-ref>
    <pkg-ref id="com.gvnraudio.thegvnr.app" version="1.0.0">TheGVNR-App.pkg</pkg-ref>
</installer-gui-script>
DISTXML

cat > "$INSTALLER_DIR/welcome.txt" << 'WELCOME'
Welcome to The GVNR v1.0.0

The GVNR is a cutting-edge hybrid distortion and multi-effect
audio plugin for macOS. Features include:

  - 30 distortion algorithms with multi-polar processing
  - 24 morphing filter types (pre and post distortion)
  - 3-band multiband crossover with per-band routing
  - Physical modeling feedback with pitch snap and scale quantization
  - Modulation: envelope follower, morphing LFO, 2 curve sequencers
  - 50 factory presets across 10 categories
  - Apple-inspired dark UI with Play and Advanced views

Compatible with Logic Pro X (AU), Ableton Live (VST3),
and FL Studio (VST3).

This installer includes:
  - VST3 plugin  -->  /Library/Audio/Plug-Ins/VST3/
  - Audio Unit   -->  /Library/Audio/Plug-Ins/Components/
  - Standalone   -->  /Applications/

Requires macOS 10.13 or later. Native Apple Silicon support.
WELCOME

echo "[5/5] Building final installer..."
productbuild --distribution "$INSTALLER_DIR/distribution.xml" \
             --package-path "$INSTALLER_DIR" \
             --resources "$INSTALLER_DIR" \
             "$INSTALLER_DIR/TheGVNR-Installer-v${VERSION}.pkg"

# Cleanup
rm -rf "$STAGING"
rm -f "$INSTALLER_DIR/TheGVNR-VST3.pkg"
rm -f "$INSTALLER_DIR/TheGVNR-AU.pkg"
rm -f "$INSTALLER_DIR/TheGVNR-App.pkg"
rm -f "$INSTALLER_DIR/distribution.xml"
rm -f "$INSTALLER_DIR/welcome.txt"

echo ""
echo "=== Done! ==="
echo "Installer: $INSTALLER_DIR/TheGVNR-Installer-v${VERSION}.pkg"
echo ""
echo "To install, double-click the .pkg file or run:"
echo "  sudo installer -pkg \"$INSTALLER_DIR/TheGVNR-Installer-v${VERSION}.pkg\" -target /"
