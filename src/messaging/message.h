/******************************************************************************/
/*!
\file   message.h
\author Ryan hanson
\par    email: ryan.hanson\@digipen.edu
\par    Course: GAM200
\brief
 deprecated message class, used to passing messages to other parts of the engine

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

//// DEPRECATED CODE ////

//
//#ifndef MESSAGE_H
//#define MESSAGE_H
//
//#include <string>
//
//class Message
//{
//public:
//  // Types of messages
//  enum class MessageType
//  {
//    InputEvent = 0,
//    SoundEvent,
//    CollisionEvent,
//  };
//
//  /**************************************************************************/
//  /*!
//  \brief Constructor for base Message
//  */
//  /**************************************************************************/
//  Message(MessageType type);
//
//  /**************************************************************************/
//  /*!
//  \brief Destructor for Message
//  */
//  /**************************************************************************/
//  virtual ~Message();
//
//  /**************************************************************************/
//  /*!
//  \brief returns the details about the message
//  */
//  /**************************************************************************/
//  std::string What() const;
//
//  /**************************************************************************/
//  /*!
//  \brief Sets the details for the message
//  \param details
//    the string with details
//  */
//  /**************************************************************************/
//  void What(std::string details);
//
//  /**************************************************************************/
//  /*!
//  \brief Gets the type of the message
//  \return type_
//    the type of the message
//  */
//  /**************************************************************************/
//  MessageType GetType() const;
//
//private:
//  std::string details_; // details about message
//
//  MessageType type_; // type of the message
//};
//
//#endif // !MESSAGE_H
