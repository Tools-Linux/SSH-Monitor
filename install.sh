#!/bin/bash

set -e

#######################################
# SSH Monitor Universal Installer
# GitHub: Docker-Update/SSH-Monitor
#######################################

APP_NAME="SSHMonitor"
SERVICE_NAME="ssh-monitor"

REPO="https://github.com/Docker-Update/SSH-Monitor.git"

SOURCE_DIR="/opt/ssh-monitor-src"
INSTALL_DIR="/opt/ssh-monitor"

SERVICE_FILE="/etc/systemd/system/${SERVICE_NAME}.service"


RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
RESET="\033[0m"


info()
{
    echo -e "${GREEN}[OK]${RESET} $1"
}

warn()
{
    echo -e "${YELLOW}[INFO]${RESET} $1"
}

error()
{
    echo -e "${RED}[ERREUR]${RESET} $1"
}


check_root()
{
    if [ "$EUID" -ne 0 ]; then
        error "Lance ce script avec sudo."
        exit 1
    fi
}


install_dependencies()
{
    info "Installation des dépendances..."

    apt update

    apt install -y \
        git \
        cmake \
        build-essential \
        pkg-config \
        libcurl4-openssl-dev \
        nlohmann-json3-dev
}


clone_or_update()
{
    if [ -d "$SOURCE_DIR/.git" ]; then

        warn "Dépôt existant détecté."

        cd "$SOURCE_DIR"

        git pull

    else

        warn "Clonage du dépôt..."

        rm -rf "$SOURCE_DIR"

        git clone "$REPO" "$SOURCE_DIR"

    fi
}


compile()
{
    info "Compilation..."

    cd "$SOURCE_DIR"

    mkdir -p build

    cd build

    cmake ..

    make -j"$(nproc)"
}


install_binary()
{
    info "Installation du programme..."

    mkdir -p "$INSTALL_DIR"


    # Recherche du binaire compilé
    BINARY_PATH=$(find "$SOURCE_DIR/build" -type f -executable -name "$APP_NAME" | head -n 1)


    if [ -z "$BINARY_PATH" ]; then
        error "Binaire introuvable."
        find "$SOURCE_DIR/build" -type f
        exit 1
    fi


    info "Binaire trouvé : $BINARY_PATH"


    # Installation du binaire
    cp "$BINARY_PATH" "$INSTALL_DIR/$APP_NAME"

    chmod +x "$INSTALL_DIR/$APP_NAME"



    # Installation du config.json
    if [ -f "$INSTALL_DIR/config.json" ]; then

        warn "config.json existant détecté, conservation."

    else

        if [ -f "$SOURCE_DIR/config.json" ]; then

            cp "$SOURCE_DIR/config.json" "$INSTALL_DIR/config.json"

            info "config.json installé."

        else

            warn "Aucun config.json trouvé dans le repository."

        fi

    fi


    info "Installation terminée."
}


create_service()
{

info "Création du service systemd..."


cat > "$SERVICE_FILE" <<EOF
[Unit]
Description=SSH Monitor
After=network.target


[Service]
Type=simple
WorkingDirectory=$INSTALL_DIR
ExecStart=$INSTALL_DIR/$APP_NAME
Restart=always
RestartSec=5
User=root


[Install]
WantedBy=multi-user.target
EOF


systemctl daemon-reload

}


start_service()
{
    systemctl enable "$SERVICE_NAME"
    systemctl restart "$SERVICE_NAME"

    info "Service démarré."
}


uninstall()
{
    warn "Suppression de SSH Monitor..."


    systemctl stop "$SERVICE_NAME" 2>/dev/null || true
    systemctl disable "$SERVICE_NAME" 2>/dev/null || true


    rm -f "$SERVICE_FILE"

    systemctl daemon-reload


    rm -rf "$INSTALL_DIR"
    rm -rf "$SOURCE_DIR"


    info "SSH Monitor supprimé."
}



install()
{
    install_dependencies
    clone_or_update
    compile
    install_binary
    create_service


    echo
    echo "Démarrer le service maintenant ?"
    echo "1) Oui"
    echo "2) Non"

    read -p "Choix : " START


    if [ "$START" = "1" ]; then
        start_service
    fi


    info "Installation terminée."
}



menu()
{

while true
do

echo
echo "================================"
echo "       SSH Monitor"
echo "================================"
echo
echo "1) Installer"
echo "2) Mettre à jour"
echo "3) Désinstaller"
echo "4) Quitter"
echo


read -p "Choix : " CHOICE


case $CHOICE in

1)
    install
    ;;

2)
    install
    ;;

3)
    uninstall
    ;;

4)
    exit 0
    ;;

*)
    error "Choix invalide."
    ;;

esac


done

}



check_root
menu