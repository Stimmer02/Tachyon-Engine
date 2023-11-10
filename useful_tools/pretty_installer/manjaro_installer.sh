#!/bin/bash

PROJECT_NAME="tachion-engine"
LSP_P1="0647f1fa233fffa14e938de99dbc4ff8  ./LSP_config_p1"
LSP_P2="eea610f346f9a7551ce69e2a33362e4a  ./LSP_config_p2"

if ! test -d  ~/.local/share/mime/packages/; then
  echo "Directory does not exists - creating new one."
  mkdir -p ~/.local/share/mime/packages/

  if ! test -d  ~/.local/share/mime/packages/; then
    echo "Fail: Could not create directory"
    exit 1
  fi
fi

echo "Copying needed files..."
cp ./text-pykn.xml ~/.local/share/mime/packages/
sudo cp ./Engine-Teal.png /usr/share/icons
echo "Copying done"

echo "Updating MIME database..."
update-mime-database ~/.local/share/mime
xdg-mime default kate.desktop text/pykn
echo "Updating done"


echo "Searching for $PROJECT_NAME root directory..."

DIR=$(pwd)
PARENT_DIR="$(dirname "$DIR")"
PARENT_DIR="$(dirname "$PARENT_DIR")"
ROOT_DIR=$(find "$PARENT_DIR" -type d -name "$PROJECT_NAME")


if [ "$ROOT_DIR" = "" ]; then
  echo "Could not find root directory, searching from ~/ folder, first fitting directory will be used"
  ROOT_DIR=$(find ~/ -type d -name "$PROJECT_NAME")
fi

if [ "$ROOT_DIR" = "" ]; then
  echo "Fail: Could not find root directory"
  exit 1
fi
echo "Root found in: $ROOT_DIR"
echo "Creating LSP.json config file..."

MD5_SUM=$(md5sum -z ./LSP_config_p1 | tr -d '\0')
echo "$MD5_SUM"
if [ "$MD5_SUM" != "$LSP_P1" ]; then
  echo "Modified file may not work properly"
fi
MD5_SUM=$(md5sum -z ./LSP_config_p2 | tr -d '\0')
echo "$MD5_SUM"
if [ "$MD5_SUM" != "$LSP_P2" ]; then
  echo "Modified file may not work properly"
fi

cat ./LSP_config_p1 > LSP.json
echo -n "$ROOT_DIR" >> LSP.json
cat ./LSP_config_p2 >> LSP.json
echo "LSP.json done"


echo "All done!"

echo ""
echo "To apply LSP.json config you must open Kate Settings->Configure Kate->LSP Settings->Use Server Settings. Then select LSP.json file in field: Settings File."

echo ""
echo "To enable Kate to properly hilight .pykn files open Kate and select Tools->Highlighting->Sources->OpenCL."

