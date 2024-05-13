#include "fs_metadata_collection.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::document::view_or_value;

using namespace mongo;

const value FSMetadataCollectionEntry::to_document() {
    int final_size = -1;
    if(file_size.has_value()) {
        final_size = file_size.value();
    }

    return make_document(kvp(INODE_KEY, inode), kvp(PARENT_DIR_INODE_KEY, parent_dir_inode), kvp(BASE_NAME_KEY, base_name),

    // md type
    kvp(FILE_TYPE_KEY, file_type),

    // user perms
    kvp(USER_MODE_READ_KEY, mode.user_perm.read), kvp(USER_MODE_WRITE_KEY, mode.user_perm.write),
    kvp(USER_MODE_EXEC_KEY, mode.user_perm.exec),

    // group perms
    kvp(GROUP_MODE_READ_KEY, mode.user_perm.read), kvp(GROUP_MODE_WRITE_KEY, mode.user_perm.write),
    kvp(GROUP_MODE_EXEC_KEY, mode.user_perm.exec),

    // univ perms
    kvp(UNIV_MODE_READ_KEY, mode.user_perm.read), kvp(UNIV_MODE_WRITE_KEY, mode.user_perm.write),
    kvp(UNIV_MODE_EXEC_KEY, mode.user_perm.exec),

    // set timestamps
    kvp(LAST_ACCESS_KEY, last_access), kvp(LAST_MODIFY_KEY, last_modify), kvp(LAST_CHANGE_KEY, last_change),

    // set id's
    kvp(UID_KEY, (int)uid), kvp(GID_KEY, (int)gid),

    // file/dir size
    kvp(FILE_SIZE_KEY, final_size));
}

void FSMetadataCollection::remove_mtn_dir() {
    INODE root_inode = fs::FSHelper::get_inode_of_root_fs();
    Connection conn;
    auto ms_data_collection = GET_FS_METADATA_COLLECTION(&conn);

    ms_data_collection.delete_one(
    make_document(kvp(FSMetadataCollectionEntry::PARENT_DIR_INODE_KEY, (INODE)PARENT_DIR_INODE_OF_ROOT_FS_DIR)));
}

void FSMetadataCollection::update_md_entry_size(INODE inode, int size) {
    Connection conn;
    auto collection = GET_FS_METADATA_COLLECTION(&conn);

    collection.update_one(make_document(kvp(FSMetadataCollectionEntry::INODE_KEY, inode)),
    make_document(kvp("$set", make_document(kvp(FSMetadataCollectionEntry::FILE_SIZE_KEY, size)))));
}

FSMetadataCollectionEntry::FSMetadataCollectionEntry(const char* _base_name, mode_t _mode, MDFileType _file_type) {
    inode = MongoManager::generate_id();

    base_name = std::string(_base_name);
    file_type = (_file_type == MDFileType::FILE) ? FSMetadataCollectionEntry::FILE_TYPE_FILE_VALUE :
                                                   FSMetadataCollectionEntry::FILE_TYPE_DIR_VALUE;
    mode      = fs::Mode{_mode};

    auto fuse_context = fuse_get_context();
    auto cur_time     = time(NULL);
    last_access       = cur_time;
    last_change       = cur_time;
    last_modify       = cur_time;

    uid = (int)fuse_context->uid;
    gid = (int)fuse_context->gid;
}

std::optional<INODE> FSMetadataCollection::create_entry(const char* path, FSMetadataCollectionEntry& fs_metadata_collection_entry) {
    // NOTE: veryfing valid path and getting parent inodec
    auto parent_dir_inode_opt = reach_parent_inode(path);

    if(!parent_dir_inode_opt.has_value()) {
        return std::nullopt;
    }

    auto parent_dir_inode = parent_dir_inode_opt.value();

    auto inode = create_entry_with_dir_parent(parent_dir_inode, fs_metadata_collection_entry);

    return inode;
}

std::optional<INODE> FSMetadataCollection::create_entry_with_dir_parent(INODE dir_parent_inode,
FSMetadataCollectionEntry& fs_metadata_collection_entry) {
    fs_metadata_collection_entry.set_parent_dir_inode(dir_parent_inode);

    // NOTE: setting init size
    if(fs_metadata_collection_entry.file_type == FSMetadataCollectionEntry::FILE_TYPE_FILE_VALUE) {
        fs_metadata_collection_entry.file_size = 0;

        // NOTE: creating data and lookup entry for block 0
        CollectionHelper::create_next_data_lookup_entries(fs_metadata_collection_entry.inode);
    }

    Connection conn;
    auto md_collection = GET_FS_METADATA_COLLECTION(&conn);
    auto doc           = fs_metadata_collection_entry.to_document();
    auto doc_result    = md_collection.insert_one(doc.view());

    assert(doc_result);

    return fs_metadata_collection_entry.inode;
}

std::optional<FSMetadataCollectionEntry> FSMetadataCollection::search_by_inode(INODE inode) {
    Connection conn;
    auto md_collection = GET_FS_METADATA_COLLECTION(&conn);
    auto doc           = md_collection.find_one(make_document(kvp(FSMetadataCollectionEntry::INODE_KEY, inode)));

    if(!doc.has_value()) {
        return std::nullopt;
    }

    return FSMetadataCollectionEntry::bson_to_md_entry(doc.value());
}

std::optional<FSMetadataCollectionEntry> FSMetadataCollection::search_by_base_name(std::string& base_name) {
    Connection conn;
    auto md_collection = GET_FS_METADATA_COLLECTION(&conn);
    auto doc = md_collection.find_one(make_document(kvp(FSMetadataCollectionEntry::BASE_NAME_KEY, base_name)));

    if(!doc.has_value()) {
        return std::nullopt;
    }

    return FSMetadataCollectionEntry::bson_to_md_entry(doc.value());
}

// https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_stat.h.html
const mode_t FSMetadataCollectionEntry::to_mode_t() {
    mode_t res = 0;

    // user bits
    if(mode.user_perm.read) {
        res = res | S_IRUSR;
    }
    if(mode.user_perm.write) {
        res = res | S_IWUSR;
    }
    if(mode.user_perm.exec) {
        res = res | S_IXUSR;
    }

    // group bits
    if(mode.group_perm.read) {
        res = res | S_IRGRP;
    }
    if(mode.group_perm.write) {
        res = res | S_IWGRP;
    }
    if(mode.group_perm.exec) {
        res = res | S_IXGRP;
    }

    // univ bits
    if(mode.univ_perm.read) {
        res = res | S_IROTH;
    }
    if(mode.univ_perm.write) {
        res = res | S_IWOTH;
    }
    if(mode.univ_perm.exec) {
        res = res | S_IXOTH;
    }

    if(file_type == FILE_TYPE_FILE_VALUE) {
        return S_IFREG | res;
    }

    return S_IFDIR | res;
}

bool FSMetadataCollection::inode_is_parent(INODE parent_dir_inode, INODE child_inode) {
    Connection conn;
    auto fs_metadata_collection = GET_FS_METADATA_COLLECTION(&conn);

    auto doc_bson = fs_metadata_collection.find_one(make_document(kvp(FSMetadataCollectionEntry::INODE_KEY, child_inode)));
    assert(doc_bson);

    auto fs_metadata_collection_entry = FSMetadataCollectionEntry::bson_to_md_entry(doc_bson.value());

    return fs_metadata_collection_entry.get_parent_dir_inode() == parent_dir_inode;
}

std::vector<FSMetadataCollectionEntry> FSMetadataCollection::get_child_md_entries_of_parent_dir(INODE parent_dir_inode) {
    std::vector<FSMetadataCollectionEntry> res{};
    Connection conn;
    auto fs_metadata_collection = GET_FS_METADATA_COLLECTION(&conn);
    auto docs_bson =
    fs_metadata_collection.find(make_document(kvp(FSMetadataCollectionEntry::PARENT_DIR_INODE_KEY, parent_dir_inode)));

    for(auto doc_bson : docs_bson) {
        auto fs_metadata_collection_entry = FSMetadataCollectionEntry::bson_to_md_entry(doc_bson);
        res.push_back(fs_metadata_collection_entry);
    }

    return res;
}

std::optional<FSMetadataCollectionEntry>
FSMetadataCollection::get_child_entry_from_parent_inode(INODE parent_dir_inode, std::string& child_base_name) {
    std::vector<FSMetadataCollectionEntry> children = get_child_md_entries_of_parent_dir(parent_dir_inode);

    for(auto child : children) {
        if(child.base_name == child_base_name) {
            return child;
        }
    }

    return std::nullopt;
}

FSMetadataCollectionEntry FSMetadataCollection::get_md_entry_of_fs_root_dir() {
    INODE root_inode = fs::FSHelper::get_inode_of_root_fs();
    auto entry_opt   = search_by_inode(root_inode);
    assert(entry_opt.has_value());
    auto entry = entry_opt.value();

    return entry;
}

std::optional<FSMetadataCollectionEntry> FSMetadataCollection::get_entry_from_path(const char* path) {
    std::string base_name = fs::FSHelper::get_base_name_of_path(path);
    auto parent_inode_opt = reach_parent_inode(path);

    if(!parent_inode_opt.has_value()) {
        std::cerr << "ERROR: parent inode was inode from path: " << path << " found!" << std::endl;
        return std::nullopt;
    }

    if(parent_inode_opt.value() == 0) {
        std::cout << "fs root dir was specified by path" << std::endl;
        return get_md_entry_of_fs_root_dir();
    }

    auto parent_inode = parent_inode_opt.value();

    auto children = get_child_md_entries_of_parent_dir(parent_inode);
    for(auto child : children) {
        if(child.base_name == base_name) {
            return child;
        }
    }

    return std::nullopt;
}

std::optional<INODE> FSMetadataCollection::reach_parent_inode(const char* path) {
    std::vector<std::string> path_components = fs::FSHelper::get_path_components(path);
    assert(path_components.size() >= 1);

    if(path_components.size() == 1) {
        std::cout << "path components vector of size 1: " << path_components[0] << std::endl;

        if(path_components[0] == "/") {
            std::cout << "fs root dir specified by reach_parent_inode" << std::endl;
            return PARENT_DIR_INODE_OF_ROOT_FS_DIR;
        }

        std::cerr << "invalid path: " << path << std::endl;
        assert(false);
    }

    // NOTE: removing base file name
    path_components.pop_back();

    // NOTE: checking parent->child for path components until base dir
    INODE cur_parent_dir_inode = fs::FSHelper::get_inode_of_root_fs();
    for(int i = 0; i < path_components.size(); i++) {
        if(i == path_components.size() - 1) {
            break;
        }

        auto child_entry_opt = get_child_entry_from_parent_inode(cur_parent_dir_inode, path_components[i + 1]);
        if(!child_entry_opt.has_value()) {
            std::cerr << path_components[i + 1] << " was not a child of inode: " << cur_parent_dir_inode << std::endl;
            return std::nullopt;
        }
        auto child_entry     = child_entry_opt.value();
        cur_parent_dir_inode = child_entry.inode;

        // FIXME: convert to enum when deserializing from bson
        if(child_entry.file_type != std::string("dir")) {
            std::cerr << "child inode: " << child_entry.inode << " was not a dir entry" << std::endl;
        }
    }

    return cur_parent_dir_inode;
}

FSMetadataCollectionEntry FSMetadataCollectionEntry::bson_to_md_entry(view bson_doc) {
    fs::Mode mode(fs::Perm{bson_doc[USER_MODE_READ_KEY].get_bool(), bson_doc[USER_MODE_WRITE_KEY].get_bool(),
                  bson_doc[USER_MODE_EXEC_KEY].get_bool()},
    fs::Perm{bson_doc[GROUP_MODE_READ_KEY].get_bool(), bson_doc[GROUP_MODE_WRITE_KEY].get_bool(),
    bson_doc[GROUP_MODE_EXEC_KEY].get_bool()},
    fs::Perm{bson_doc[UNIV_MODE_READ_KEY].get_bool(), bson_doc[UNIV_MODE_WRITE_KEY].get_bool(),
    bson_doc[UNIV_MODE_EXEC_KEY].get_bool()});

    time_t last_access = bson_doc[LAST_ACCESS_KEY].get_int64();
    time_t last_modify = bson_doc[LAST_MODIFY_KEY].get_int64();
    time_t last_change = bson_doc[LAST_CHANGE_KEY].get_int64();

    INODE inode            = bson_doc[INODE_KEY].get_int32().value;
    INODE parent_dir_inode = bson_doc[PARENT_DIR_INODE_KEY].get_int32();

    int uid = bson_doc[UID_KEY].get_int32();
    int gid = bson_doc[GID_KEY].get_int32();

    int file_size                      = bson_doc[FILE_SIZE_KEY].get_int32();
    std::optional<int> final_file_size = std::nullopt;

    if(file_size != -1) {
        final_file_size = file_size;
    }

    auto entry = FSMetadataCollectionEntry(inode, std::string(bson_doc[BASE_NAME_KEY].get_string()),
    std::string(bson_doc[FILE_TYPE_KEY].get_string()), mode, time(&last_access), time(&last_modify), time(&last_change),
    uid, gid, final_file_size);

    entry.set_parent_dir_inode(parent_dir_inode);

    return entry;
}
