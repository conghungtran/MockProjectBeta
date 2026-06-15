#pragma once

namespace PrinterHub {
	namespace Core {
		enum class PrinterStatus {
			ACTIVE,
			INSERVICE,
			RETIRED,
		};

		enum class PrinterBrand {
			HP,
			CANON,
			EPSON,
			OTHER,
		};

		enum class PrinterPriority {
			LOW,
			MEDIUM,
			HIGH,
			CRITICAL
		};
	}
}