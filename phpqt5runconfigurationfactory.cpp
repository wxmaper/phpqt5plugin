#include "phpqt5runconfigurationfactory.h"
#include "phpqt5project.h"
#include "phpqt5runconfiguration.h"

#include "phpqt5constants.h"

#include <debugger/debuggerrunconfigurationaspect.h>
#include <projectexplorer/target.h>

#include <memory>

using namespace ProjectExplorer;

namespace PHPQt5 {

PHPQt5RunConfigurationFactory::PHPQt5RunConfigurationFactory()
{
}

QList<Core::Id> PHPQt5RunConfigurationFactory::availableCreationIds(Target *parent,
                                                                 IRunConfigurationFactory::CreationMode mode) const
{
    Q_UNUSED(mode);
    if (canHandle(parent))
        return { Constants::C_PHPQt5RUNCONFIGURATION_ID };
    return {};
}

QString PHPQt5RunConfigurationFactory::displayNameForId(Core::Id id) const
{
    QString result = id.toString() + QStringLiteral("-TempRunConf");
    return result;
}

bool PHPQt5RunConfigurationFactory::canCreate(Target *parent, Core::Id id) const
{
    Q_UNUSED(id);
    return canHandle(parent);
}

bool PHPQt5RunConfigurationFactory::canRestore(Target *parent, const QVariantMap &map) const
{
    Q_UNUSED(parent);
    Q_UNUSED(map);
    return canHandle(parent);
}

bool PHPQt5RunConfigurationFactory::canClone(Target *parent, RunConfiguration *product) const
{
    QTC_ASSERT(parent, return false);
    QTC_ASSERT(product, return false);
    return true;
}

RunConfiguration *PHPQt5RunConfigurationFactory::clone(Target *parent, RunConfiguration *product)
{
    QTC_ASSERT(parent, return nullptr);
    QTC_ASSERT(product, return nullptr);
    std::unique_ptr<PHPQt5RunConfiguration> result(new PHPQt5RunConfiguration(parent, Constants::C_PHPQt5RUNCONFIGURATION_ID));
    return result->fromMap(product->toMap()) ? result.release() : nullptr;
}

bool PHPQt5RunConfigurationFactory::canHandle(Target *parent) const
{
    Q_UNUSED(parent);
    return qobject_cast<PHPQt5Project *>(parent->project());
}

RunConfiguration *PHPQt5RunConfigurationFactory::doCreate(Target *parent, Core::Id id)
{
    Q_UNUSED(id);
    auto result = new PHPQt5RunConfiguration(parent, id);
    return result;
}

RunConfiguration *PHPQt5RunConfigurationFactory::doRestore(Target *parent, const QVariantMap &map)
{
    Q_UNUSED(map);
    auto result = new PHPQt5RunConfiguration(parent, idFromMap(map));
    result->fromMap(map);
    return result;
}

}
