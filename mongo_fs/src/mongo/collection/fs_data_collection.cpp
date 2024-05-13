#include "fs_data_collection.h"

using namespace mongo;

std::optional<FS_DATA_ID> FSDataCollection::create_entry(FSDataCollectionEntry& fs_data_collection_entry) {
    Connection conn;
    auto file_collection = GET_FS_DATA_COLLECTION(&conn);

    b_binary data{binary_sub_type::k_binary, static_cast<std::uint32_t>(fs::FSConfig::BLOCK_SIZE),
    reinterpret_cast<const std::uint8_t*>(fs_data_collection_entry.buf)};

    std::cout << "creating entry for fs_data_collection" << std::endl;
    int fs_data_id = MongoManager::generate_id();
    file_collection.insert_one(
    make_document(kvp(FSDataCollectionEntry::FS_DATA_ID_KEY, fs_data_id), kvp(FSDataCollectionEntry::FS_BUF_KEY, data)));

    return fs_data_id;
}

void FSDataCollection::update_entry(FSDataCollectionEntry& fs_data_collection_entry) {
    Connection conn;
    auto collection = GET_FS_DATA_COLLECTION(&conn);

    b_binary data{binary_sub_type::k_binary, static_cast<std::uint32_t>(fs::FSConfig::BLOCK_SIZE),
    reinterpret_cast<const std::uint8_t*>(fs_data_collection_entry.buf)};

    collection.update_one(make_document(kvp(FSDataCollectionEntry::FS_DATA_ID_KEY, fs_data_collection_entry.fs_data_id.value())),
    make_document(kvp("$set", make_document(kvp(FSDataCollectionEntry::FS_BUF_KEY, data)))));
}

FSDataCollectionEntry FSDataCollectionEntry::bson_to_entry(value bson_doc) {
    const char* bytes    = (const char*)bson_doc[FSDataCollectionEntry::FS_BUF_KEY].get_binary().bytes;
    char* writable_bytes = new char[fs::FSConfig::BLOCK_SIZE];

    for(int i = 0; i < fs::FSConfig::BLOCK_SIZE; i++) {
        writable_bytes[i] = bytes[i];
    }

    return FSDataCollectionEntry{bson_doc[FSDataCollectionEntry::FS_DATA_ID_KEY].get_int32(), writable_bytes};
}

std::optional<FSDataCollectionEntry> FSDataCollection::read_entry(FS_DATA_ID fs_id) {
    Connection conn;
    auto data_collection = GET_FS_DATA_COLLECTION(&conn);

    auto bson_res = data_collection.find_one(make_document(kvp(FSDataCollectionEntry::FS_DATA_ID_KEY, fs_id)));

    assert(bson_res);

    if(!bson_res.has_value()) {
        return std::nullopt;
    }

    auto bson_doc = bson_res.value();
    return FSDataCollectionEntry::bson_to_entry(bson_doc);
}

void FSDataCollection::remove_entry(FS_DATA_ID fs_data_id) {
  Connection conn;
  auto data_collection = GET_FS_DATA_COLLECTION(&conn);
  data_collection.delete_one(make_document(kvp(FSDataCollectionEntry::FS_DATA_ID_KEY, fs_data_id)));
}
