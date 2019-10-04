# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# An old map warning, see https://en.wikipedia.org/wiki/Here_be_dragons
about-config-warning-title = Audaciosamente indo, onde ninguém jamais esteve!
about-config-warning-text = Alterar essas configurações avançadas pode prejudicar a estabilidade, segurança e desempenho desta aplicação. Você só deve continuar se estiver seguro do que está fazendo.
about-config-warning-checkbox = Sempre mostrar este aviso
about-config-warning-button = Eu aceito o risco
about-config-title = about:config
about-config2-title = Configurações avançadas
about-config-search-input =
    .placeholder = Pesquisar
about-config-show-all = Mostrar tudo
about-config-pref-add = Adicionar
about-config-pref-toggle = Alternar
about-config-pref-edit = Editar
about-config-pref-save = Salvar
about-config-pref-reset = Redefinir
about-config-pref-delete = Excluir

## Labels for the type selection radio buttons shown when adding preferences.

about-config-pref-add-type-boolean = Booleano
about-config-pref-add-type-number = Número
about-config-pref-add-type-string = String

## Preferences with a non-default value are differentiated visually, and at the
## same time the state is made accessible to screen readers using an aria-label
## that won't be visible or copied to the clipboard.
##
## Variables:
##   $value (String): The full value of the preference.

about-config-pref-accessible-value-default =
    .aria-label = { $value } (padrão)
about-config-pref-accessible-value-custom =
    .aria-label = { $value } (personalizado)
