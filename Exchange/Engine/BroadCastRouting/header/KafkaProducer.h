/*
 * KafkaProducer.h
 *
 *  Created on: Apr 19, 2025
 *      Author: mukesh
 */

#ifndef KAFKAPRODUCER_H_
#define KAFKAPRODUCER_H_

#include <librdkafka/rdkafkacpp.h>
#include <iostream>
#include <memory>

class KafkaProducer {
public:
    KafkaProducer(const std::string& brokers, const std::string& topic_name) {
        std::string errstr;

        // Create configuration
        RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
        conf->set("bootstrap.servers", brokers, errstr);
        conf->set("queue.buffering.max.messages", "100000", errstr);     // Max messages in buffer
        conf->set("queue.buffering.max.kbytes", "1048576", errstr);      // 1 GB buffer
        conf->set("batch.num.messages", "1000", errstr);                 // Up to 1000 messages per batch
        conf->set("queue.buffering.max.ms", "5", errstr);                // Wait up to 5ms to batch


        // Create producer
        producer = RdKafka::Producer::create(conf, errstr);
        if (!producer) {
            std::cerr << "Failed to create producer: " << errstr << std::endl;
            exit(1);
        }

        topic = RdKafka::Topic::create(producer, topic_name, nullptr, errstr);
        if (!topic) {
            std::cerr << "Failed to create topic: " << errstr << std::endl;
            exit(1);
        }

        this->topic_name = topic_name;
        std::cout<<"\nTopic Created:-"<<topic_name<<std::endl;
    }

    //void send(const char* data, size_t size){
    //void send(const std::string& data){
    void send(const std::string& json) {
        RdKafka::ErrorCode resp = producer->produce(
            topic, RdKafka::Topic::PARTITION_UA,
            RdKafka::Producer::RK_MSG_COPY,
            //const_cast<char*>(data.c_str()), data.size(),
	        //(void*)data, size,  // send buffer
			const_cast<char*>(json.data()), json.size(),
            nullptr, nullptr);

        if (resp != RdKafka::ERR_NO_ERROR) {
            std::cerr << "Send failed: " << RdKafka::err2str(resp) << std::endl;
        } else {
            std::cout << "Message sent to topic " << topic_name << std::endl;
        }

        producer->poll(0); // Serve delivery reports, events, etc.
    }

    ~KafkaProducer() {
        producer->flush(1000);
        delete topic;
        delete producer;
    }

private:
    RdKafka::Producer* producer;
    RdKafka::Topic* topic;
    std::string topic_name;
};






#endif /* KAFKAPRODUCER_H_ */
