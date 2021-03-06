#include "QuickStreamsTest.hpp"

// the attachOperator case verifies, that a stream is executed
// once the stream it's attached to is closed and the data is passed correctly.
void QuickStreamsTest::attach() {
	Trigger cpCreated;
	Trigger cpAttached;

	QList<QString> awakeningOrder;
	QVariant passedData;

	auto createdStream = streams->create([&](
		const StreamHandle& stream,
		const QVariant& data
	) {
		Q_UNUSED(data)
		awakeningOrder.append("created");
		stream.close("testdata");
		cpCreated.trigger();
	});

	auto attachedStream = createdStream->attach([&](
		const QVariant& data
	) {
		passedData = data;
		awakeningOrder.append("attached");
		cpAttached.trigger();
		return QVariant();
	});

	Q_UNUSED(attachedStream)

	QVERIFY(cpCreated.wait(100));
	QVERIFY(cpAttached.wait(100));

	// Ensure attached stream was executed after the created one
	QCOMPARE(awakeningOrder[0], QString("created"));
	QCOMPARE(awakeningOrder[1], QString("attached"));

	// Ensure both streams have only been executed once,
	// and the failure stream was not executed at all
	QCOMPARE(cpCreated.count(), 1);
	QCOMPARE(cpAttached.count(), 1);

	// Verify the data passed from the createdStream to the attachedStream
	QCOMPARE(passedData, QVariant("testdata"));
}
