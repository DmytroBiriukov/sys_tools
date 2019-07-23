#ifndef __CNV_HPP_INCLUDED__   
#define __CNV_HPP_INCLUDED__   

#include <string>
#include <vector>
#include <iostream>

namespace sys_tools {

    #define UNDEFINED_LOG_FIELD "undefined"
    #define NO_HEADER_ERROR_CODE 7
    #define FAILED_WITH_STRING_ERROR_CODE 5
    #define FAILED_WITH_INT_ERROR_CODE 3

    void generate_alphabet(char* ABC){
      int i = 0;
      ABC[i++] = '-';
      for(char c='0'; c<='9'; c++) ABC[i++]=c;
      for(char c='A'; c<='Z'; c++) ABC[i++]=c;
      ABC[i++] = '_';
      for(char c='a'; c<='z'; c++) ABC[i++]=c;
    }

    struct TLogStruct{
      std::string bidderId;
      std::string transactionId;
      uint64_t requestedAt;
      uint32_t adSourceId;
      uint32_t adTagId;
      std::string publisherSource;
      uint16_t environment;
      uint16_t bidDuration;
      uint16_t bidFloor;
      uint16_t bidRate;
      uint16_t activity;
      uint16_t standard;
      /// Default c-tor
      TLogStruct(){
          requestedAt = 0;
          adSourceId = 0;
          adTagId = 0;
          environment = 0;
          bidDuration = 0;
          bidFloor = 0;
          bidRate = 0;
          activity = 0;
          standard = 0;
          bidderId.reserve(32);
          transactionId.reserve(16);
          publisherSource.reserve(48);
          bidderId=std::string(UNDEFINED_LOG_FIELD);
          transactionId=std::string(UNDEFINED_LOG_FIELD);
          publisherSource=std::string(UNDEFINED_LOG_FIELD);
      }
      /// Init c-tor
      TLogStruct(std::string b_, uint64_t r_,
                uint32_t at_, uint16_t e_, uint16_t s_)
          :bidderId(b_), requestedAt(r_),
            adTagId(at_), environment(e_), standard(s_)
      { adSourceId = 0;
        char ABC[64];
        generate_alphabet(ABC);
        transactionId.clear();        
        unsigned int mask = 0;
        mask = ~((~mask) << 6);
        uint64_t value = r_;
        int i = 0;
        for(i = 0; i < 11 && value; i++){
           transactionId.push_back(ABC[value & mask]);
           value = value >> 6;
        }
        transactionId.resize(i);
        publisherSource = std::string(UNDEFINED_LOG_FIELD);
      }

      TLogStruct(std::string b_, std::string t_, uint64_t r_,
                uint32_t at_, uint16_t e_, uint16_t s_)
          :bidderId(b_), transactionId(t_), requestedAt(r_),
            adTagId(at_), environment(e_), standard(s_)
      { adSourceId = 0;
        publisherSource = std::string(UNDEFINED_LOG_FIELD);
      }
      /// Copy c-tor
      TLogStruct(const TLogStruct& l){
          requestedAt = l.requestedAt;
          adSourceId = l.adSourceId;
          adTagId = l.adTagId;
          environment = l.environment;
          bidDuration = l.bidDuration;
          bidFloor = l.bidFloor;
          bidRate = l.bidRate;
          activity = l.activity;
          standard = l.standard;
          bidderId = l.bidderId;
          transactionId = l.transactionId;
          publisherSource = l.publisherSource;
      }

    };




    // -- conversion handlers
    void string_field_handler(std::string &byte_buffer_, void *ptr_)
    {
        byte_buffer_.push_back( ((std::string *)ptr_)->length() );
        for(auto c : (*(std::string *)ptr_)){
            byte_buffer_.push_back(c);
        }
    }
    int to_string_field_handler(std::string::iterator &begin_, std::string::iterator end_, void* ptr_)
    {
        uint8_t size  = (uint8_t)(*begin_++);
        if(ptr_ != NULL && ptr_ != nullptr) {
            ((std::string*)ptr_)->clear();
        }else {
            printf("\nEmpty memory!");
        }

        if(size > (end_ - begin_)) return FAILED_WITH_STRING_ERROR_CODE;

        while(size-- && begin_ != end_)((std::string*)ptr_)->push_back( *begin_++ );
        return 0;
    }

    void int_n_t_field_handler(std::string &byte_buffer_, void *ptr_, short n){
        uint8_t* c_ptr = (uint8_t*) ptr_;
        for(short i=0; i<n; i++) byte_buffer_.push_back(*c_ptr++);    
    }
    void int64_t_field_handler(std::string &byte_buffer_, void *ptr_){
        int_n_t_field_handler(byte_buffer_, ptr_, (short)8);
    }
    void int32_t_field_handler(std::string &byte_buffer_, void *ptr_){
        int_n_t_field_handler(byte_buffer_, ptr_, (short)4);
    }
    void int16_t_field_handler(std::string &byte_buffer_, void *ptr_){
        int_n_t_field_handler(byte_buffer_, ptr_, (short)2);
    }
    void int8_t_field_handler(std::string &byte_buffer_, void *ptr_){
        byte_buffer_.push_back( *((uint8_t*) ptr_) );
    }
    int to_int_n_t_field_handler(std::string::iterator &begin_, std::string::iterator end_, void* ptr_, short n)
    {   uint8_t* char_ptr = (uint8_t*) ptr_;
        if(n > (end_ - begin_)) return FAILED_WITH_INT_ERROR_CODE;
        for(short i=0; i<n && begin_!= end_; i++)  *char_ptr++ = *begin_++;
        return 0;
    }
    int to_int64_t_field_handler(std::string::iterator &begin_, std::string::iterator end_, void* ptr_){
        return to_int_n_t_field_handler(begin_, end_, ptr_, (short)8);
    }
    int to_int32_t_field_handler(std::string::iterator &begin_, std::string::iterator end_, void* ptr_){
        return to_int_n_t_field_handler(begin_, end_, ptr_, (short)4);
    }
    int to_int16_t_field_handler(std::string::iterator &begin_, std::string::iterator end_, void* ptr_){
        return to_int_n_t_field_handler(begin_, end_, ptr_, (short)2);
    }
    int to_int8_t_field_handler(std::string::iterator &begin_, std::string::iterator end_, void* ptr_){
        return to_int_n_t_field_handler(begin_, end_, ptr_, (short)1);
    }

    // --- conversion processor
    int bytes_to_log_struct (std::string &bytes_, TLogStruct* ptr_struct_)
    {
        struct p_connector_ {
            void * field_ptr;
            // handler that process string (bytes) from iterator and forward according data type
            int (*handler)(std::string::iterator &begin_, std::string::iterator end_, void* ptr_);
        };
        std::vector< p_connector_ > connection_points =
        {  { &(ptr_struct_->bidderId), to_string_field_handler },
        { &(ptr_struct_->transactionId), to_string_field_handler },
        { &(ptr_struct_->requestedAt), to_int64_t_field_handler },
        { &(ptr_struct_->adSourceId), to_int32_t_field_handler },
        { &(ptr_struct_->adTagId), to_int32_t_field_handler },
        { &(ptr_struct_->publisherSource), to_string_field_handler },
        { &(ptr_struct_->environment), to_int16_t_field_handler },
        { &(ptr_struct_->bidDuration), to_int16_t_field_handler },
        { &(ptr_struct_->bidFloor), to_int16_t_field_handler },
        { &(ptr_struct_->bidRate), to_int16_t_field_handler },
        { &(ptr_struct_->activity), to_int16_t_field_handler },
        { &(ptr_struct_->standard), to_int16_t_field_handler }
        };
        std::string::iterator it = bytes_.begin();
        std::string::iterator end_ = bytes_.end();

        if(it == end_) return NO_HEADER_ERROR_CODE;

        for(char c='A'; c<'E' && it != end_; c++, it++)
            if( (it == end_) || (*it != c) ) return NO_HEADER_ERROR_CODE;

        unsigned short error_code = 0;
        for(auto &point_ : connection_points){
            if(error_code = point_.handler(it, end_, point_.field_ptr)){
            return error_code;
            }
        }
        return 0;
    }
    /// --- Conversion processor ---
    void log_struct_to_bytes(std::string &bytes_, TLogStruct* ptr_struct_)
    {
            struct connector_ {
                void * field_ptr;
                /// handler that receives string (bytes) and pointer to data (either int or string, or any other)
                void (*handler)(std::string &, void*);
            };
            std::vector< connector_ > connection_points =
            {  { &(ptr_struct_->bidderId), string_field_handler },
            { &(ptr_struct_->transactionId), string_field_handler },
            { &(ptr_struct_->requestedAt), int64_t_field_handler },
            { &(ptr_struct_->adSourceId), int32_t_field_handler },
            { &(ptr_struct_->adTagId), int32_t_field_handler },
            { &(ptr_struct_->publisherSource), string_field_handler },
            { &(ptr_struct_->environment), int16_t_field_handler },
            { &(ptr_struct_->bidDuration), int16_t_field_handler },
            { &(ptr_struct_->bidFloor), int16_t_field_handler },
            { &(ptr_struct_->bidRate), int16_t_field_handler },
            { &(ptr_struct_->activity), int16_t_field_handler },
            { &(ptr_struct_->standard), int16_t_field_handler }
            };
            /// Include correctness mark
            for(char c='A'; c<'E'; c++) bytes_.push_back( c );
            /// Process conversion for every field
            for(auto &point_ : connection_points)
                point_.handler(bytes_, point_.field_ptr);
    }

    void print_log_struct(TLogStruct* log_)
    {

        std::cout<<std::endl<<"bidderId : "<<log_->bidderId<<std::endl
                            <<"transactionId : "<<log_->transactionId<<std::endl
                            <<"requestedAt : "<<log_->requestedAt<<" millisec from beginning of epoch"                                
                                <<  std::endl
                            <<"activity : "<<(uint)log_->activity<<"  [pre_bid_error=0, win=1, imp=2, bid=3, post_bid_error=4, opp=5, noop=6]"<<std::endl
                            <<"environment : "<<(uint)log_->environment<<" [AD_PLACE_SITE = 1, AD_PLACE_APP = 2, AD_PLACE_TV = 16] "<<std::endl
                            <<"publisherSource : "<<log_->publisherSource<<std::endl
                            <<"bidDuration : "<<(uint)log_->bidDuration<<" millisec"<<std::endl
                            <<"bidFloor : "<<(uint)log_->bidFloor<<" (100xCPM)"<<std::endl
                            <<"bidRate : "<<(uint)log_->bidRate<<" (100xCPM)"<<std::endl
                            <<"standard : "<< ((log_->standard == 0)?"vast":"ortb")<<std::endl
                            <<"adTagId : "<<(uint)log_->adTagId<<std::endl
                            <<"adSourceId : "<<(uint)log_->adSourceId<<std::endl;
    }

    const char* verbolize_error(unsigned short error_code){
        switch(error_code){
            case NO_HEADER_ERROR_CODE : return "NO_HEADER_ERROR_CODE"; break;
            case FAILED_WITH_STRING_ERROR_CODE : return "FAILED_WITH_STRING_ERROR_CODE"; break;
            case FAILED_WITH_INT_ERROR_CODE : return "FAILED_WITH_INT_ERROR_CODE"; break;
            default: return "UNKNOWN ERROR"; break; 
        }        
    }

}

#endif