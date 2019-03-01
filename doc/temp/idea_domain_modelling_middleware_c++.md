Middleware for generically and safely, in C++, do the cascading subdomains pattern, with extensions for Qt integration and what not.

class User : dom::object
{

}

class Account : dom::object
{
    User owner;
    dom::Address more;
    dom::String cenas;
}

class Domain : dom::domain
{
    dom::array<Account> accounts;
}


dom::array<Account> myAccounts = domain.accounts.select([](Account& a){ return a.age > 18;});

dom::object<Account> myAccount = myAccounts.select([](Account& a) { return a.name == "Me"; });

// Create a mock, which fills placeholder data in everything, taking into account metatypes like Name and Address, and making placeholders that make sense for those types
dom::mock mocker = new dom::mock(domain);
mocker->setDensity(dom::mock::dense);
mocker->setCustomResolver(); // Something to override mutations with custom behavior
mocker->setCustomGenerator<DragonName>(myDragonNameGenerator);
mocker->populate();