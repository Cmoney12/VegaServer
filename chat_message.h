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

    uint8_t* head() {
        return header;
    }

    std::size_t length() const
    {
        return HEADER_LENGTH + body_length_;
    }

    uint8_t* body()
    {
        return data_.get() + HEADER_LENGTH;
    }

    int body_length() const
    {
        return body_length_;
    }

    void set_size(const std::size_t& size) {
        body_length_ = size;
        data_ = std::make_unique<uint8_t[]>(size + HEADER_LENGTH);
    }

    const char* get_username(const uint8_t *_data, std::size_t size) {

        bson_t *received_bson;
        const bson_t *received;
        bson_reader_t *reader;
        bson_iter_t iter;
        const char *user = nullptr;
        bool reached_eof = false;


        received_bson = bson_new_from_data(_data, size);

        for(int i = 0; i < 50; i++) {
            std::cout << (data_.get() + 5)[i];
        }

        /**if (!bson_validate (received_bson, BSON_VALIDATE_NONE, &err_offset)) {
            fprintf (stderr,
                     "The document failed to validate at offset: %u\n",
                     (unsigned) err_offset);
        }**/

        /**if ((json = bson_as_canonical_extended_json (received_bson, nullptr))) {
            printf ("%s\n", json);
            bson_free ((void *) json);
        }**/

        //reader = bson_reader_new_from_data(_data, size);
        //received = bson_reader_read(reader, &reached_eof);

        if (bson_iter_init_find(&iter, received_bson, "Receiver") && BSON_ITER_HOLDS_UTF8(&iter)) {
            user = bson_iter_utf8(&iter, nullptr);
        }

        //int sizes = sizeof(*data_.get());
        //bson_reader_destroy(reader);

        return user;

    }

    bool decode_header() {
        //body_length_ = std::atoi((char*)header);
        std::memcpy(&body_length_, header, sizeof body_length_);
        set_size(body_length_);
        std::memcpy(data_.get(), header, HEADER_LENGTH);
        data_.get()[3] = (body_length_>>24) & 0xFF;
        data_.get()[2] = (body_length_>>16) & 0xFF;
        data_.get()[1] = (body_length_>>8) & 0xFF;
        data_.get()[0] = body_length_ & 0xFF;
        std::cout << "Body Size " << body_length_ << std::endl;
        if(body_length_ > MAX_MESSAGE_SIZE) {
            body_length_ = 0;
            std::cout << "Body Size " << body_length_ << std::endl;
            return false;
        }
        return true;
    }


private:
    int body_length_{};
    std::unique_ptr<uint8_t[]> data_;
    enum { MAX_MESSAGE_SIZE = 999999 };
    uint8_t header[HEADER_LENGTH]{};
};

#endif //VEGASERVER_CHAT_MESSAGE_H
