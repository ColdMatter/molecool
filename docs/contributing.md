Contributing to _molecool_
==========================

Thanks for your interest in contributing to _molecool_! :tada: We love getting [pull requests](https://www.quora.com/GitHub-What-is-a-pull-request) for bugfixes and contributions.
These guidelines are intended to help smooth that process, and allow us to review and approve your changes quickly and easily. 
Improvements are always welcome! Feel free to [open an issue][issue-tracker] or [submit a new pull request][submit-pr].

If you're new to [GitHub][github], you may want to begin with [Getting Started with GitHub](https://help.github.com/en/categories/getting-started-with-github) and Thinkful's [GitHub Pull Request Tutorial](https://www.thinkful.com/learn/github-pull-request-tutorial/).

## Reporting Bugs

Bugs should be reported on our [GitHub Issue Tracker][issue-tracker] using the bug report template.
Follow the advice in [How do I ask a good question?][how-to-ask]. While the article is intended for people asking questions on Stack Overflow, it all applies to writing a good bug report too.

## Requesting New Features

Feature requests should also be sent to our [GitHub Issue Tracker][issue-tracker] using the feature request template.

- Explain the problem that you're having, and anything you've tried to solve it using the currently available features.

- Explain how this new feature will help.

- If possible, provide an example, like a code snippet, showing what your new feature might look like in use.

Also, much of the advice in [How do I ask a good question?][how-to-ask] applies here too.

## Contributing a Fix or Feature

You've created a new fix or feature for molecool. Awesome!

1. If you haven't already, create a fork of the repository.

2. Create a topic branch, and make all of your changes on that branch.

3. Submit a pull request using the implemented [pull request template][pr-template].

4. Give us a moment. _molecool_ is maintained by only a few people, so it may take us a bit to review your request. 

If you're not sure what any of that means, check out Thinkful's [GitHub Pull Request Tutorial][thinkful-pr-tutorial] for a complete walkthrough of the process.

### Writing a Good Pull Request

- Stay focused on a single fix or feature. If you submit multiple changes in a single request, we may like some but spot issues with others. 
When that happens, we have to reject the whole thing. If you submit each change in its own request it is easier for us to review and approve.

- Limit your changes to only what is required to implement the fix or feature. In particular, avoid style or formatting tools that may modify the formatting of other areas of the code.

- Use descriptive commit titles/messages. "Implemented \<feature\>" or "Fixed \<problem\> is better than "Updated \<file\>".

- Make sure the code you submit compiles and runs without issues.

- Use [closing keywords][github-help-closing-keywords] in the appropriate section of our Pull Request template where applicable.

- Follow our coding conventions, which we've intentionally kept quite minimal.

### Coding Conventions

- Naming convention:
  - For functions we use camel case: **`functionName`**.
  - For (scoped) variables and function parameters we use camel case: **`variableName`** and **`parameterName`**.

  - For class names we use pascal case: **`ClassName`**.

  - For class variables we use the Hungarian notation:
    - Class member variables get the 'm_' prefix: **`m_ClassMemberVariableName`**.
    - Class static variables get the 's_' prefix: **`s_ClassStaticVariableName`**.

  - For macros we use snake case: **`MACRO_NAME`**.
    - If it is specifically related to _molecool_, we add the 'MC_' prefix: **`MC_MACRO_NAME`**.
    - If there is a macro for the application and for the engine, we add an additional 'CORE_' prefix to the engine macro:  **`MC_CORE_MACRO_NAME`**.

- Use tabs for indentation, not spaces.

- When in doubt, match the code that's already there.



[github]: https://github.com
[how-to-ask]: https://stackoverflow.com/help/how-to-ask
[issue-tracker]: https://github.com/ColdMatter/molecool/issues
[submit-pr]: https://github.com/ColdMatter/molecool/pulls
[pr-template]: https://github.com/ColdMatter/molecool/notes/pull_request_template.md
[thinkful-pr-tutorial]: https://www.thinkful.com/learn/github-pull-request-tutorial/
[github-help-closing-keywords]: https://help.github.com/en/articles/closing-issues-using-keywords
