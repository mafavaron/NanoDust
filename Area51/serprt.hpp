#ifndef SERPRT_HPP_
#define SERPRT_HPP_

int writeport(int fd, unsigned char *chars, int numBytes);
int readport(int fd, unsigned char *result);
int getbaud(int fd);

#endif /*SERPRT_HPP_*/
