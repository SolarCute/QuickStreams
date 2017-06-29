#include "QuickStreamsTest.hpp"

// Verify the error received by the failure stream
// is the same as the error thrown in the failed stream
void QuickStreamsTest::failure_data_stdRuntimeError() {
	Trigger cpCreated;
	Trigger cpFailure;

	QVariant receivedError;

	auto createdStream = streams->create([&](
		const StreamHandle& stream, const QVariant& data
	) {
		Q_UNUSED(data)
		cpCreated.trigger();
		throw std::runtime_error("something went wrong");
		stream.close();
	});

	auto failureStream = createdStream->failure([&](const QVariant& error) {
		receivedError = error;
		cpFailure.trigger();
		return QVariant();
	});

	Q_UNUSED(failureStream)

	QVERIFY(cpCreated.wait(100));
	QVERIFY(cpFailure.wait(100));

	// Ensure the error received by the failure stream
	// is the same as the error thrown in the created stream
	QCOMPARE(receivedError, QVariant("something went wrong"));
}
