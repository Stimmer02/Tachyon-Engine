#!/bin/bash

PROJECT_NAME="tachion-engine"
DIR=$(pwd)
LSP_P1="219c35052880ced1aa5bc57c32ba4db1  ./LSP_config_p1"
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
echo ""


echo "Searching for $PROJECT_NAME root directory..."

DIR=$(pwd)
PARENT_DIR="$(dirname "$DIR")"
PARENT_DIR="$(dirname "$PARENT_DIR")"
ROOT_DIR=$(find "$PARENT_DIR" -type d -name "$PROJECT_NAME")


if [ "$ROOT_DIR" = "" ]; then
  echo "Could not find root directory, searching from ~/ folder, first fitting directory will be used"
  ROOT_DIR=$(find ~/ -type d -name "$PROJECT_NAME" | head -n 1)
fi

if [ "$ROOT_DIR" = "" ]; then
  echo "Fail: Could not find root directory"
  exit 1
fi
echo "Root found in: $ROOT_DIR"
echo "Searching for compile_commands.json..."
COMPILE_COMMANDS_PATH=$(find "$ROOT_DIR" -name compile_commands.json)
if [ "$COMPILE_COMMANDS_PATH" = "" ]; then
  echo "Could not find compile_commands.json, running ./Build.sh from root directory"
  cd "$ROOT_DIR" || (echo "$ROOT_DIR does not exist"; exit 2)
  if ! test -x "$ROOT_DIR/Build.sh"; then
    echo "$ROOT_DIR/Build.sh does not exits or is not executable, cannot create compile_commands.json"
    exit 3
  fi
  "$("$ROOT_DIR"/Build.sh)"
  echo ""
  echo "Searching for compile_commands.json..."
  COMPILE_COMMANDS_PATH=$(find "$ROOT_DIR" -name compile_commands.json)
  if [ "$COMPILE_COMMANDS_PATH" = "" ]; then
    echo "Could not find compile_commands.json"
    exit 4
  fi
fi
echo "compile_commands.json found in: $COMPILE_COMMANDS_PATH"
echo ""
echo "Creating LSP.json config file..."
cd "$DIR" || (echo "$DIR does not exist!?!"; exit 5)

if ! test -r ./LSP_config_p1; then
  echo "./LSP_config_p1 does not exit"
  exit 5
fi
MD5_SUM=$(md5sum -z ./LSP_config_p1 | tr -d '\0')
echo "$MD5_SUM"
if [ "$MD5_SUM" != "$LSP_P1" ]; then
  echo "WARNING: Modified file may not work properly"
fi
if ! test -r ./LSP_config_p2; then
  echo "./LSP_config_p1 does not exit"
  exit 5
fi
MD5_SUM=$(md5sum -z ./LSP_config_p2 | tr -d '\0')
echo "$MD5_SUM"
if [ "$MD5_SUM" != "$LSP_P2" ]; then
  echo "WARNING: Modified file may not work properly"
fi

cat ./LSP_config_p1 > LSP.json
echo -n "$COMPILE_COMMANDS_PATH" >> LSP.json
cat ./LSP_config_p2 >> LSP.json
echo "LSP.json done"

echo "All done!"

echo ""
echo "To apply LSP.json config you must open Kate Settings->Configure Kate->LSP Settings->Use Server Settings. Then select LSP.json file in field: Settings File."
echo ""
echo "To enable Kate to properly hilight .pykn and .clcpp files open Kate and select Settings->Configure Kate->Open/Save->Modes & Filetypes then in the Filetype field find Sources/OpenCL and add ;*.clcpp;*.pykn to the file extensions field"
echo ""
echo "You can also create custom Filetype, if so then .pykn MIME type is text/pykn"


