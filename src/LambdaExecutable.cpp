#include "LambdaExecutable.hpp"
#include <QVariant>
#include <exception>
#include <QSharedPointer>

quickstreams::LambdaExecutable::LambdaExecutable(Function function) :
	_function(function)
{}

void quickstreams::LambdaExecutable::execute(const QVariant& data) {
	// If the function is null then close the stream referencing this handle
	// because otherwise it would try to execute it causing a segfault
	if(!_function) {
		_handle->close(data);
		return;
	}

	// Try to execute
	try {
		// Execute and forward closure responsibility to the user
		_function(*_handle, data);
	} catch(const std::exception& error) {
		_error.reset(new QVariant(error.what()));
	} catch(...) {
		_error.reset(new QVariant());
	}
}
