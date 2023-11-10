#!/bin/bash

if ! test -d  ~/.local/share/mime/packages/; then
  echo "Directory does not exists - creating new one."
  mkdir -p ~/.local/share/mime/packages/

  if ! test -d  ~/.local/share/mime/packages/; then
    echo "Could not create directory"
    exit 1
  fi
fi

echo "Copying needed files..."
cp ./text-pykn.xml ~/.local/share/mime/packages/
sudo cp ./Engine-Teal.png /usr/share/icons
echo "Copying done!"

echo "Updating MIME database..."
update-mime-database ~/.local/share/mime
xdg-mime default kate.desktop text/pykn
echo "Updating done!"


echo "All done!"

echo ""
echo "To enable Kate to properly hilight .pykn files open Kate and select Tools->Highlighting->Sources->OpenCL."

