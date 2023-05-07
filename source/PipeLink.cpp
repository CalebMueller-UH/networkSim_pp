#include "PipeLink.hpp"

void PipeLink::connect() {
  // Create the first pipe
  if (pipe(_end1_pipefd) == -1) {
    throw std::runtime_error("Failed to create pipe for end1");
  }

  // Create the second pipe
  if (pipe(_end2_pipefd) == -1) {
    // Clean up the first pipe if the second pipe fails
    close(_end1_pipefd[0]);
    close(_end1_pipefd[1]);
    throw std::runtime_error("Failed to create pipe for end2");
  }
}

std::string PipeLink::recv(int recipientId, size_t maxLen) {
  int read_fd;
  if (this->get_end1() == recipientId) {
    read_fd = _end1_pipefd[dirRecv];
  } else if (this->get_end2() == recipientId) {
    read_fd = _end2_pipefd[dirRecv];
  } else {
    std::cerr << "Invalid link" << std::endl;
    this->_log.log("Invalid Link");
    return 0;
  }

  // Allocate buffer to read msg into
  char buffer[maxLen];

  // Read the message from pipe into buffer
  int bytesRead = read(read_fd, buffer, maxLen - 1);
  if (bytesRead > 0) {
    return buffer;
  } else {
    return nullptr;
  }
}

int PipeLink::send(int senderId, const std::string& msg) {
  int write_fd;
  if (this->get_end1() == senderId) {
    write_fd = _end1_pipefd[dirSend];
  } else if (this->get_end2() == senderId) {
    write_fd = _end2_pipefd[dirSend];
  } else {
    std::cerr << "Invalid link" << std::endl;
    return 0;
  }

  int bytesWritten = write(write_fd, msg.c_str(), msg.length());
  // Check if the write was successful
  if (bytesWritten == -1) {
    std::cerr << "Error writing to pipe!\n";
    return 0;
  }
  return 1;
}
