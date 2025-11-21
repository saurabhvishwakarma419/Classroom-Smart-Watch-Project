#!/bin/bash

# Smart Classroom Watch - Restore Script
# Restores from backup

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Configuration
BACKUP_DIR=${BACKUP_DIR:-./backups}
BACKUP_FILE=$1

print_status() {
    echo -e "${GREEN}✓${NC} $1"
}

print_error() {
    echo -e "${RED}✗${NC} $1"
}

print_info() {
    echo -e "${YELLOW}ℹ${NC} $1"
}

if [ -z "$BACKUP_FILE" ]; then
    echo -e "${RED}Usage: $0 <backup_file.tar.gz>${NC}"
    echo -e "\nAvailable backups:"
    ls -lh $BACKUP_DIR/smart-classroom-backup-*.tar.gz | tail -10
    exit 1
fi

if [ ! -f "$BACKUP_FILE" ]; then
    print_error "Backup file not found: $BACKUP_FILE"
    exit 1
fi

echo -e "${BLUE}================================${NC}"
echo -e "${BLUE}Smart Classroom Watch - Restore${NC}"
echo -e "${BLUE}================================${NC}\n"

# Warning
echo -e "${RED}⚠️  WARNING: This will overwrite existing data!${NC}"
read -p "Are you sure you want to restore from backup? (yes/no): " -r
if [[ ! $REPLY =~ ^yes$ ]]; then
    echo "Restore cancelled"
    exit 0
fi

# Verify checksum
CHECKSUM_FILE="${BACKUP_FILE%.tar.gz}.sha256"
if [ -f "$CHECKSUM_FILE" ]; then
    print_info "Verifying checksum..."
    sha256sum -c $CHECKSUM_FILE
    print_status "Checksum verified"
fi

# Extract backup
TEMP_RESTORE_DIR="/tmp/restore_$(date +%s)"
mkdir -p $TEMP_RESTORE_DIR
print_info "Extracting backup..."
tar -xzf $BACKUP_FILE -C $TEMP_RESTORE_DIR
print_status "Backup extracted"

# Find backup directory
BACKUP_NAME=$(ls $TEMP_RESTORE_DIR)
RESTORE_PATH="${TEMP_RESTORE_DIR}/${BACKUP_NAME}"

# Restore MongoDB
if [ -d "${RESTORE_PATH}/mongodb" ]; then
    echo -e "\n${BLUE}Restoring MongoDB...${NC}"
    print_info "Dropping existing database..."
    mongo smart_classroom --eval "db.dropDatabase()"
    
    print_info "Restoring MongoDB..."
    mongorestore --gzip --dir="${RESTORE_PATH}/mongodb"
    print_status "MongoDB restored"
fi

# Restore Configuration
echo -e "\n${BLUE}Restoring configuration...${NC}"
if [ -f "${RESTORE_PATH}/config/backend.env" ]; then
    cp ${RESTORE_PATH}/config/backend.env backend/.env
    print_status "Backend configuration restored"
fi

# Restore uploads
if [ -d "${RESTORE_PATH}/uploads" ]; then
    echo -e "\n${BLUE}Restoring uploaded files...${NC}"
    rm -rf backend/uploads
    cp -r ${RESTORE_PATH}/uploads backend/
    print_status "Uploaded files restored"
fi

# Clean up
print_info "Cleaning up..."
rm -rf $TEMP_RESTORE_DIR
print_status "Cleanup completed"

echo -e "\n${GREEN}================================${NC}"
echo -e "${GREEN}Restore Complete! 🎉${NC}"
echo -e "${GREEN}================================${NC}\n"

print_info "Please restart services:"
echo "${BLUE}cd backend && npm restart${NC}"
