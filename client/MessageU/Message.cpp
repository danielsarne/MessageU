#include "Message.h"
#include "Request.h"
#include "Consts.h"
#include <string>


string Message::packed() {
	string packedMessage = "";
	packedMessage += this->dstUserID;
	append_to_string(packedMessage, this->type);
	append_to_string(packedMessage, this->contentSize);
	if (this->contentSize > 0) {
		packedMessage += this->content;
	}
	return packedMessage;
}

ostream& operator<<(ostream& outs, const Message& m) {
	cout << "From: " << m.srcUserName << endl;
	cout << "Content:" << endl;
	switch (m.type) {
	case GET_SYM_KEY_MESSAGE_TYPE_CODE:
		cout << "Request for a symmetric key." << endl;
		break;
	case SEND_TEXT_MESSAGE_TYPE_CODE:
		cout << m.content << endl;
		break;
	case SEND_SYM_KEY_MESSAGE_TYPE_CODE:
		cout << "Got symetric key." << endl;
		break;
	}
	cout << "------EOM------" << endl;
	return outs;
}