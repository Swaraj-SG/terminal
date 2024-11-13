// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "Extensions.h"
#include "Extensions.g.cpp"
#include "ExtensionsViewModel.g.cpp"

#include <LibraryResources.h>
#include "..\WinRTUtils\inc\Utils.h"

using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::UI::Xaml::Navigation;

namespace winrt::Microsoft::Terminal::Settings::Editor::implementation
{
    Extensions::Extensions()
    {
        InitializeComponent();
    }

    void Extensions::OnNavigatedTo(const NavigationEventArgs& e)
    {
        _ViewModel = e.Parameter().as<Editor::ExtensionsViewModel>();
    }

    void Extensions::ExtensionLoaded(const Windows::Foundation::IInspectable& /*sender*/, const Windows::UI::Xaml::RoutedEventArgs& /*args*/)
    {
        // TODO CARLOS: re-enable this code
        //const auto& toggleSwitch = sender.as<Controls::ToggleSwitch>();
        //const auto& extensionSource = toggleSwitch.Tag().as<hstring>();
        //toggleSwitch.IsOn(_ViewModel.GetExtensionState(extensionSource));
    }

    void Extensions::ExtensionToggled(const Windows::Foundation::IInspectable& /*sender*/, const Windows::UI::Xaml::RoutedEventArgs& /*args*/)
    {
        // TODO CARLOS: re-enable this code
        //const auto& toggleSwitch = sender.as<Controls::ToggleSwitch>();
        //const auto& extensionSource = toggleSwitch.Tag().as<hstring>();
        //_ViewModel.SetExtensionState(extensionSource, toggleSwitch.IsOn());
    }

    ExtensionsViewModel::ExtensionsViewModel(const Model::CascadiaSettings& settings) :
        _settings{ settings },
        _fragmentExtensions{ settings.FragmentExtensions() },
        _profilesModified{ single_threaded_vector<Model::Profile>() },
        _profilesAdded{ single_threaded_vector<Model::Profile>() },
        _colorSchemesAdded{ single_threaded_vector<Model::ColorScheme>() }
    {
        for (const auto&& fragExt : _fragmentExtensions)
        {
            for (const auto&& profile : fragExt.ModifiedProfilesView())
            {
                _profilesModified.Append(profile);
            }

            for (const auto&& profile : fragExt.NewProfilesView())
            {
                _profilesAdded.Append(profile);
            }

            for (const auto&& scheme : fragExt.ColorSchemesView())
            {
                _colorSchemesAdded.Append(scheme);
            }
        }
    }

    bool ExtensionsViewModel::GetExtensionState(hstring extensionSource) const
    {
        uint32_t ignored;
        return !_DisabledProfileSources().IndexOf(extensionSource, ignored);
    }

    void ExtensionsViewModel::SetExtensionState(hstring extensionSource, bool enableExt)
    {
        uint32_t idx;
        auto disabledExtensions = _DisabledProfileSources();
        const bool currentlyDisabled = disabledExtensions.IndexOf(extensionSource, idx);

        // current status mismatches the desired status,
        // update the list of disabled extensions
        if (currentlyDisabled == enableExt)
        {
            if (enableExt)
            {
                disabledExtensions.RemoveAt(idx);
            }
            else
            {
                disabledExtensions.Append(extensionSource);
            }
        }
    }
}
