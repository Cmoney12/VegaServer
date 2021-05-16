//
// Created by corey on 5/16/21.
//

#ifndef VEGASERVER_CHAT_MESSAGE_H
#define VEGASERVER_CHAT_MESSAGE_H

#include <bson/bson.h>


class chat_message : std::enable_shared_from_this<chat_message> {
public:
    enum { HEADER_LENGTH = 4 };

    chat_message():body_length_(0){}

    uint8_t* data() const
    {
        return data_.get();
    }

    const uint8_t* data()
    {
        return data_.get();
    }

    char* head() {
        return header;
    }

    std::size_t length() const
    {
        return HEADER_LENGTH + body_length_;
    }

    uint8_t* body() const
    {
        return data_.get() + HEADER_LENGTH;
    }

    std::size_t body_length() const
    {
        return body_length_;
    }

    void set_size(std::size_t size) {
        body_length_ = size;
        data_ = std::make_unique<uint8_t[]>(size);
    }

    static const char* get_username(const uint8_t *data, std::size_t size) {

        const bson_t *received;
        bson_reader_t *reader;
        bson_iter_t iter;
        size_t size1 = 0;
        const char* user = nullptr;

        reader = bson_reader_new_from_data(data, size);

        received = bson_reader_read(reader, nullptr);

        if (bson_iter_init_find(&iter, received, "Receiver") && BSON_ITER_HOLDS_UTF8(&iter)) {
            user = bson_iter_utf8(&iter, nullptr);
        }

        bson_reader_destroy(reader);

        return user;

    }

    bool decode_header() {
        body_length_ = std::atoi((char*)header);
        std::cout << "size " << body_length_ << std::endl;
        set_size(body_length_);
        std::memcpy(data_.get(), header, HEADER_LENGTH);
        if(body_length_ > MAX_MESSAGE_SIZE) {
            body_length_ = 0;
            return false;
        }
        return true;
    }


private:
    std::size_t body_length_{};
    std::unique_ptr<uint8_t[]> data_;
    enum { MAX_MESSAGE_SIZE = 9999 };
    char header[HEADER_LENGTH + 1]{};
};


#endif //VEGASERVER_CHAT_MESSAGE_H
