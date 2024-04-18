/**
 * @file messages.h
 * @brief Observer message values
 */

#if !defined(INC_MESSAGES_H__)
#define INC_MESSAGES_H__

#define SENDER_ID_NONE        (-1)
#define SENDER_ID_UNKNOWN     (255)
#define SENDER_ID_INTERNET    (1)
#define SENDER_ID_LED         (2)
#define SENDER_ID_LOGGER      (3)
#define SENDER_ID_OBSERVER    (4)
#define SENDER_ID_RTC         (5)
#define SENDER_ID_STORAGE     (6)

#define MESSAGE_ID_NONE       (-1)
#define MESSAGE_ID_INTERNET   (1)
#define MESSAGE_ID_STORAGE    (6)

#endif /* INC_MESSAGES_H__ */