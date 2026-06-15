// ICommand.h
#pragma once
#include <string>

namespace PrinterHub {
    namespace Core {
        enum class CommandType {
            Add,
            Edit,
            Delete,
            Clear
        };

        class ICommand {
        public:
            virtual ~ICommand() = default;
            virtual void execute() = 0;
            virtual void undo() = 0;
            virtual CommandType getType() const = 0;
            virtual std::string getDescription() const = 0;
        };
    }
}