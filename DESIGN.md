# Design

Overview of design patterns used, including collaborator roles.

## Filter.hpp (Template Method : Abstract Class)
### Collaborators:
+ MacroFilter.hpp (Concrete Class)
+ OutputFilter.hpp (Concrete Class)
+ ParserFilter.hpp (Concrete Class)
+ StatisticsFilter.hpp (Concrete Class)
+ XPathFilter.hpp (Concrete Class)
+ XMLDocument.hpp (Dependency)

## FilterFactory.hpp (Factory Method : Creator)
### Collaborators:
+ Filter.hpp (Abstract Class)
+ XMLDocument.hpp (Dependency)

## MacroFilter.hpp (Template Method/Command : Concrete Class/Macro)
### Collaborators:
+ Filter.hpp (Abstract Class)
+ XMLDocument.hpp (Dependency)

## OutputFilter.hpp (Template Method : Concrete Class)
### Collaborators:
+ Filter.hpp (Abstract Class)
+ XMLDocument.hpp (Dependency)

## ParserFilter.hpp (Template Method : Concrete Class)
### Collaborators:
+ Filter.hpp (Abstract Class)
+ XMLDocument.hpp (Dependency)

## StatisticsFilter.hpp (Template Method : Concrete Class)
### Collaborators:
+ Filter.hpp (Abstract Class)
+ XMLDocument.hpp (Dependency)

## XMLPipeline.hpp (Strategy : Strategy)
### Collaborators:
+ MacroFilter.hpp (Dependency)

## XPathFilter.hpp (Template Method : Concrete Class)
### Collaborators:
+ Filter.hpp (Abstract Class)
+ XMLDocument.hpp (Dependency)

## XMLDocumentHandlerParser.hpp (Template Method : Concrete Class)
### Collaborators:
+ XMLDocumentParser.hpp (Abstract Class)

## XMLDocumentParser.hpp (Template Method : Abstract Class)
### Collaborators:
+ XMLDocumentHandlerParser.hpp (Concrete Class)
+ XMLDocumentTemplateParser.hpp (Concrete Class)
+ XMLDocument.hpp (Dependency)

## XMLDocumentTemplateParser.hpp (Template Method : Concrete Class)
### Collaborators:
+ XMLDocumentParser.hpp (Abstract Class)

## XMLParser.hpp (Facade : Parser)
### Collaborators:
+ XMLParserHandler.hpp (Dependency)

## XMLParserDocumentHandler.hpp (Template Method : Concrete Class)
### Collaborators:
+ XMLParserHandler (Abstract Class)

## XMLParserFactory.hpp (Factory Method/Singleton : Creator)
### Collaborators:
+ XMLDocumentParser.hpp (Dependency)

## XMLParserHandler.hpp (Template Method : Abstract Class)
### Collaborators:
+ XMLParserDocumentHandler (Concrete Class)

## XPathEvaluator.hpp (Command : Command)
### Collaborators:
+ XMLDocument.hpp (Dependency)
